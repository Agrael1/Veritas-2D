#include "Color.h"
#include "GSBase.h"
#include "PSBase.h"
#include "VSBase.h"
#include "IndexedTriangleList.h"
#include "VLine.h"
#include "Class.h"
#include "Standard.h"

IAOut ia;

inline void DrawFlatTriangle(selfptr,
	SVMVECTOR* it0,
	SVMVECTOR* it1,
	SVMVECTOR* it2,
	FVMVECTOR* dv0,
	FVMVECTOR* dv1,
	VMVECTOR* itEdge1)
{
	const UINT size = self->VS->VSOutSize;
	VMVECTOR* _P = _alloca(size);

	// create edge interpolant for left edge (always v0)
	VMVECTOR* itEdge0 = _alloca(size);
	memcpy_s(itEdge0, self->VS->VSOutSize, it0, size);

	// calculate start and end scanlines
	const int yStart = max((int)ceilf(it0->c.y - 0.5f), 0);
	const int yEnd = min((int)ceilf(it2->c.y - 0.5f), (int)self->gfx->nFrameHeight - 1); // the scanline AFTER the last line drawn
																						 // do interpolant prestep
	const float step = ((float)yStart + 0.5f - it0->c.y);
	VSOutScale(_P, dv0, step, size);
	VSOutAdd(itEdge0, itEdge0, _P, size);

	VSOutScale(_P, dv1, step, size);
	VSOutAdd(itEdge1, itEdge1, _P, size);


	for (int y = yStart; y < yEnd; y++,
		VSOutAdd(itEdge0, itEdge0, dv0,size),
		VSOutAdd(itEdge1, itEdge0, dv1,size))
	{
		// calculate start and end pixels
		const int xStart = max((int)ceilf(itEdge0->m128_f32[0] - 0.5f), 0);
		const int xEnd = min((int)ceilf(itEdge1->m128_f32[0] - 0.5f), (int)self->gfx->nFrameLength - 1); // the pixel AFTER the last pixel drawn

																										// create scanline interpolant startpoint
																										// (some waste for interpolating x,y,z, but makes life easier not having
																										//  to split them off, and z will be needed in the future anyways...)
		VMVECTOR* iLine = _alloca(size);
		memcpy_s(iLine, self->VS->VSOutSize, itEdge0, size);

		//// calculate delta scanline interpolant / dx
		const float dx = itEdge1->m128_f32[0] - itEdge0->m128_f32[0];
		VMVECTOR* diLine = _alloca(size);
		VSOutSubtract(_P, itEdge1, iLine, size);
		VSOutScale(diLine, _P, 1.0f / dx, size);

		// prestep scanline interpolant
		VSOutScale(_P, diLine, ((float)xStart + 0.5f - itEdge0->m128_f32[0]),size);
		VSOutAdd(iLine, iLine, _P, size);

		for (int x = xStart; x < xEnd; x++,
			VSOutAdd(iLine, iLine, diLine, size))
		{
			if (self->gfx->method->DepthTest(self->gfx, x, y, iLine->m128_f32[2]))
			{
				// recover interpolated z from interpolated 1/z
				const float w = 1.0f / iLine->m128_f32[3];
				// recover interpolated attributes
				// (wasted effort in multiplying pos (x,y,z) here, but
				//  not a huge deal, not worth the code complication to fix)
				VSOutScale(_P, iLine, w, size);
				// invoke pixel shader with interpolated vertex attributes
				// and use result to set the pixel color on the screen
				self->gfx->method->PrintFrame(self->gfx, x, y, self->PS->Apply(self->PS, _P));
			}
		}
	}
}
inline void DrawFlatTopTriangle2(selfptr, SVMVECTOR* it0,
	SVMVECTOR* it1,
	SVMVECTOR* it2)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	const float delta_y = 1.0f / (it2->c.y - it0->c.y);

	VMVECTOR* _P = _alloca(self->VS->VSOutSize);
	VSOutSubtract(_P, it2, it0, self->VS->VSOutSize);
	FVMVECTOR* dit0 = _alloca(self->VS->VSOutSize);
	VSOutScale(dit0, _P, delta_y, self->VS->VSOutSize);

	VSOutSubtract(_P, it2, it1, self->VS->VSOutSize);
	FVMVECTOR* dit1 = _alloca(self->VS->VSOutSize);
	VSOutScale(dit1, _P, delta_y, self->VS->VSOutSize);

	memcpy_s(_P, self->VS->VSOutSize, it1, self->VS->VSOutSize);

	// call the flat triangle render routine, right edge interpolant is it1
	DrawFlatTriangle(self, it0, it1, it2, dit0, dit1, _P);
}
inline void DrawFlatBottomTriangle2(selfptr, SVMVECTOR* it0,
	SVMVECTOR* it1,
	SVMVECTOR* it2)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	const float delta_y = 1.0f / (it2->c.y - it0->c.y);

	VMVECTOR* _P = _alloca(self->VS->VSOutSize);
	VSOutSubtract(_P, it1, it0, self->VS->VSOutSize);
	FVMVECTOR* dit0 = _alloca(self->VS->VSOutSize);
	VSOutScale(dit0, _P, delta_y, self->VS->VSOutSize);

	VSOutSubtract(_P, it2, it0, self->VS->VSOutSize);
	FVMVECTOR* dit1 = _alloca(self->VS->VSOutSize);
	VSOutScale(dit1, _P, delta_y, self->VS->VSOutSize);

	memcpy_s(_P, self->VS->VSOutSize, it0, self->VS->VSOutSize);

	// call the flat triangle render routine, right edge interpolant is it0
	DrawFlatTriangle(self, it0, it1, it2, dit0, dit1, _P);
}
void V_DrawTriangle(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2)
{
	if (v1->c.y < v0->c.y) swapptr(&v0, &v1);
	if (v2->c.y < v1->c.y) swapptr(&v1, &v2);
	if (v1->c.y < v0->c.y) swapptr(&v0, &v1);

	if (v0->c.y == v1->c.y)	// Flat Top
	{
		if (v1->c.x < v0->c.x) swapptr(&v0, &v1);
		DrawFlatTopTriangle2(self, v0, v1, v2);
	}
	if (v1->c.y == v2->c.y)	// Flat Bottom
	{
		if (v2->c.x < v1->c.x) swapptr(&v1, &v2);
		DrawFlatBottomTriangle2(self, v0, v1, v2);
	}
	else // General
	{
		const float alphaSplit =
			(v1->c.y - v0->c.y) / (v2->c.y - v0->c.y);

		SVMVECTOR* vi = _alloca(self->VS->VSOutSize);
		VSOutInterpolate(vi, v0, v2, alphaSplit, self->VS->VSOutSize);

		if (v1->c.x < vi->c.x)
		{
			DrawFlatBottomTriangle2(self, v0, v1, vi);
			DrawFlatTopTriangle2(self, v1, vi, v2);
		}
		else
		{
			DrawFlatBottomTriangle2(self, v0, vi, v1);
			DrawFlatTopTriangle2(self, vi, v1, v2);
		}
	}
}

inline void Transform(const selfptr, SVMVECTOR* v)
{
	// perform homo -> ndc on xyz / perspective-correct interpolative divide on all other attributes
	const float wInv = 1.0f / v->c.w;
	VSOutScale(v, v, wInv, self->VS->VSOutSize);
		//TODO: fix that schitt
	const float HalfViewportWidth = self->gfx->nFrameLength * 0.5f;
	const float HalfViewportHeight = self->gfx->nFrameHeight * 0.5f;

	VMVECTOR Scale = VMVectorSet(HalfViewportWidth, -HalfViewportHeight, 0.0f, 0.0f);
	VMVECTOR Offset = VMVectorSet(HalfViewportWidth, HalfViewportHeight, 0.0f, 0.0f);
	v->v = VMVectorMultiplyAdd(v->v, Scale, Offset);
	v->c.w = wInv;
}
void V_ProcessTriangle(selfptr, void* v0, void* v1, void* v2)
{
	Transform(self, v0);
	Transform(self, v1);
	Transform(self, v2);

	if (self->GS)
		self->GS->Apply(self->GS, v0, v1, v2);
	V_DrawTriangle(self, v0, v1, v2);
}

inline bool CullTest(VMVECTOR v0, VMVECTOR v1, VMVECTOR v2)
{
	VMMATRIX MC;
	MC.r[0] = v0;
	MC.r[1] = v1;
	MC.r[2] = v2;
	MC.r[3] = VMVectorZero();
	
	MC = VMMatrixTranspose(MC);
	VMVECTOR LowerBound = VMVectorNegate(MC.r[3]);
	if (!VMVector3InBounds(MC.r[0], LowerBound, MC.r[3]))
		return true;
	if (!VMVector3InBounds(MC.r[1], LowerBound, MC.r[3]))
		return true;
	if (!VMVector3InBounds(MC.r[2], VMVectorZero(), MC.r[3]))
		return true;
	return false;
}
inline void Clip1V(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2, size_t VSize)
{
	// calculate alpha values for getting adjusted vertices
	const float alphaA = (-v0->c.z) / (v1->c.z - v0->c.z);
	const float alphaB = (-v0->c.z) / (v2->c.z - v0->c.z);
	// interpolate to get v0a and v0b
	void* v0a = _alloca(VSize);
	void* v0b = _alloca(VSize);
	if (VSize != sizeof(VMVECTOR))
	{
		memcpy_s(v0a, VSize, v0 + sizeof(VMVECTOR), VSize - sizeof(VMVECTOR));
		memcpy_s(v0b, VSize, v0 + sizeof(VMVECTOR), VSize - sizeof(VMVECTOR));
	}
	VSOutInterpolate(v0a, v0, v1, alphaA, VSize);
	VSOutInterpolate(v0b, v0, v2, alphaB, VSize);

	// draw triangles
	V_ProcessTriangle(self, v0a, v1, v2);
	V_ProcessTriangle(self, v0b, v0a, v2);
}
inline void Clip2V(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2, size_t VSize)
{
	// calculate alpha values for getting adjusted vertices
	const float alpha0 = (-v0->c.z) / (v2->c.z - v0->c.z);
	const float alpha1 = (-v1->c.z) / (v2->c.z - v1->c.z);
	// interpolate to get v0a and v0b
	VSOutInterpolate(v0, v0, v2, alpha0, VSize);
	VSOutInterpolate(v1, v1, v2, alpha1, VSize);
	// draw triangles
	V_ProcessTriangle(self, v0, v1, v2);
};
void V_ClipCullTriangle(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2, size_t VSize)
{
	/*if (CullTest(v0->v, v1->v, v2->v))
		return;*/
	// cull tests
	if (v0->c.x > v0->c.w &&
		v1->c.x > v1->c.w &&
		v2->c.x > v2->c.w)
	{
		return;
	}
	if (v0->c.x < -v0->c.w &&
		v1->c.x < -v1->c.w &&
		v2->c.x < -v2->c.w)
	{
		return;
	}
	if (v0->c.y > v0->c.w &&
		v1->c.y > v1->c.w &&
		v2->c.y > v2->c.w)
	{
		return;
	}
	if (v0->c.y < -v0->c.w &&
		v1->c.y < -v1->c.w &&
		v2->c.y < -v2->c.w)
	{
		return;
	}
	if (v0->c.z > v0->c.w &&
		v1->c.z > v1->c.w &&
		v2->c.z > v2->c.w)
	{
		return;
	}
	if (v0->c.z < 0.0f &&
		v1->c.z < 0.0f &&
		v2->c.z < 0.0f)
	{
		return;
	}
	// near clipping tests
	if (v0->c.z < 0.0f)
	{
		if (v1->c.z < 0.0f)
		{
			Clip2V(self, v0, v1, v2, VSize);
		}
		else if (v2->c.z < 0.0f)
		{
			Clip2V(self, v0, v2, v1, VSize);
		}
		else
		{
			Clip1V(self, v0, v1, v2, VSize);
		}
	}
	else if (v1->c.z < 0.0f)
	{
		if (v2->c.z < 0.0f)
		{
			Clip2V(self, v1, v2, v0, VSize);
		}
		else
		{
			Clip1V(self, v1, v0, v2, VSize);
		}
	}
	else if (v2->c.z < 0.0f)
	{
		Clip1V(self, v2, v0, v1, VSize);
	}
	else // no near clipping necessary
	{
		V_ProcessTriangle(self, v0, v1, v2);
	}
}

void V_AssembleTriangles(selfptr, const void* Verts, size_t VSize, const size_t* indices, size_t indN)
{
	FVMVECTOR eyepos = VMVector4Transform((SVMVECTOR){ 0.0f,0.0f,0.0f,1.0f }.v, self->gfx->projection);

	VMVECTOR* v0p = _alloca(VSize);
	VMVECTOR* v1p = _alloca(VSize);
	VMVECTOR* v2p = _alloca(VSize);

	for (size_t i = 0; i < indN; i += 3)
	{
		memcpy_s(v0p, VSize, ((Byte*)Verts + indices[i + 0] * VSize), VSize);
		memcpy_s(v1p, VSize, ((Byte*)Verts + indices[i + 1] * VSize), VSize);
		memcpy_s(v2p, VSize, ((Byte*)Verts + indices[i + 2] * VSize), VSize);

		ia.SV_PrimID = i / 3;
		bool cull = VMVector3LessOrEqual(VMVector3Dot(
			VMVector3Cross
			(
				VMVectorSubtract(*v1p, *v0p),
				VMVectorSubtract(*v2p, *v0p)
			),  VMVectorSubtract(*v0p, eyepos)), VMVectorZero());

		// Culling by normal
		if (cull)
		{
			V_ClipCullTriangle(self, v0p, v1p, v2p, VSize);
		}
	}
}
void V_ProcessVertices(selfptr, struct IndexedTriangleList* trilist)
{
	assert(self->VS);
	void* pVSOut = _aligned_malloc(self->VS->VSOutSize * trilist->numVerts, 16);

	if (pVSOut)
	{
		// Transform all the verts accordingly
		self->VS->Apply(self->VS, pVSOut, trilist);
		V_AssembleTriangles(self, pVSOut, self->VS->VSOutSize, trilist->indices, trilist->numInds);
	}
	_aligned_free(pVSOut);
}
void V_Draw(selfptr, struct IndexedTriangleList* trilist)
{
	V_ProcessVertices(self, trilist); // Start the pipeline
}

constructMethodTable(
	.Draw = V_Draw
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->gfx = va_arg(*ap, struct Frame*);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC