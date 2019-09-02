#include "GSBase.h"
#include "PSBase.h"
#include "VSBase.h"
#include "Window.h"
#include "IndexedTriangleList.h"
#include "VLine.h"
#include "Class.h"
#include "Standard.h"

IAOut ia;

#pragma warning(disable:4133)

void DrawFlatTriangle(selfptr,
	SVMVECTOR* it0,
	SVMVECTOR* it2,
	FVMVECTOR* dv0,
	FVMVECTOR* dv1,
	VMVECTOR* itEdge1)
{
	const UINT size = self->VS.VSOutSize;
	
	// create edge interpolant for left edge (always v0)
	VMVECTOR* itEdge0 = (VMVECTOR*)it0;
	VMVECTOR* iLine = _alloca(size);
	VMVECTOR* diLine = _alloca(size);
	VMVECTOR* _P = _alloca(size);

	// calculate start and end scanlines
	const int yStart = max((int)ceil(it0->c.y - 0.5f), 0);
	const int yEnd = min((int)ceil(it2->c.y - 0.5f), (int)self->gfx->nFrameHeight - 1); // the scanline AFTER the last line drawn
																						 // do interpolant prestep
	FVMVECTOR step = _mm_set_ps1(((float)yStart + 0.5f - it0->c.y));
	for (unsigned i = 0; i < size / 16; i++)
	{
		itEdge0[i] = _mm_add_ps(itEdge0[i], _mm_mul_ps(dv0[i], step));
		itEdge1[i] = _mm_add_ps(itEdge1[i], _mm_mul_ps(dv1[i], step));
	}

	for (int y = yStart; y < yEnd; y++)
	{
		// calculate start and end pixels
		const int xStart = max((int)ceil(itEdge0->m128_f32[0] - 0.5f), 0);
		const int xEnd = min((int)ceil(itEdge1->m128_f32[0] - 0.5f), (int)self->gfx->nFrameLength - 1); // the pixel AFTER the last pixel drawn
																										// create scanline interpolant startpoint
																										// (some waste for interpolating x,y,z, but makes life easier not having
																										//  to split them off, and z will be needed in the future anyways...)
		memcpy(iLine, itEdge0, size);
		// calculate delta scanline interpolant / dx
		FVMVECTOR step2 = _mm_set_ps1((float)xStart + 0.5f - itEdge0->m128_f32[0]);
		FVMVECTOR Delta_X = _mm_sub_ps(*itEdge1, *itEdge0);
		FVMVECTOR dx = VMVectorSplatX(_mm_rcp_ss(Delta_X));
		*diLine = _mm_mul_ps(Delta_X, dx);
		*iLine = _mm_add_ps(*iLine, _mm_mul_ps(*diLine, step2));

		for (unsigned i = 1; i < size / 16; i++)
		{
			diLine[i] = _mm_mul_ps(_mm_sub_ps(itEdge1[i], itEdge0[i]), dx);
			iLine[i] = _mm_add_ps(iLine[i], _mm_mul_ps(diLine[i], step2));
		}

		const UINT premulI = y*self->gfx->nFrameLength;

		for (int x = xStart; x < xEnd; x++,
			VSOutAdd(iLine, iLine, diLine, size))
		{
			if (self->gfx->method->DepthTest(self->gfx, x, y, iLine->m128_f32[2]))
			{
				// recover interpolated z from interpolated 1/z
				FVMVECTOR w = _mm_rcp_ps(VMVectorSplatW(*iLine));
				for (unsigned i = 1; i < size / 16; i++)
					_P[i] = _mm_mul_ps(iLine[i], w);
				// invoke pixel shader with interpolated vertex attributes
				// and use result to set the pixel color on the screen
				self->gfx->localFrame[premulI + x] = self->PS->Apply(self->PS, _P);
			}
		}
		for (unsigned i = 0; i < size / 16; i++)
		{
			itEdge0[i] = _mm_add_ps(itEdge0[i], dv0[i]);
			itEdge1[i] = _mm_add_ps(itEdge1[i], dv1[i]);
		}
	}
	
}
void _DrawFlatTopTriangle(selfptr, VMVECTOR* it0, VMVECTOR* it1, VMVECTOR* it2)
{
	const UINT Size = self->VS.VSOutSize;
	VMVECTOR* _P = _alloca(Size);
	VMVECTOR* dit0 = _alloca(Size);
	VMVECTOR* dit1 = _alloca(Size);

	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	VMVECTOR d20 = _mm_sub_ps(*it2, *it0);
	VMVECTOR d21 = _mm_sub_ps(*it2, *it1);
	FVMVECTOR delta_Y = VMVectorSplatY(VMVectorDivide(g_XMOne.v, d20));

	*dit1 = _mm_mul_ps(d20, delta_Y);
	*dit0 = _mm_mul_ps(d21, delta_Y);

	for (unsigned i = 1; i < Size / 16; i++)
	{
		dit1[i] = _mm_mul_ps(_mm_sub_ps(it2[i], it0[i]), delta_Y);
		dit0[i] = _mm_mul_ps(_mm_sub_ps(it2[i], it1[i]), delta_Y);
	}
	memcpy(_P, it1, Size);

	// call the flat triangle render routine, right edge interpolant is it1
	DrawFlatTriangle(self, it0, it2, dit1, dit0, _P);
}
void _DrawFlatBottomTriangle(selfptr, VMVECTOR* it0, VMVECTOR* it1, VMVECTOR* it2)
{
	const UINT Size = self->VS.VSOutSize;
	VMVECTOR* _P = _alloca(Size);
	VMVECTOR* dit0 = _alloca(Size);
	VMVECTOR* dit1 = _alloca(Size);

	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	VMVECTOR d10 = _mm_sub_ps(*it1, *it0);
	VMVECTOR d20 = _mm_sub_ps(*it2, *it0);
	FVMVECTOR delta_Y = VMVectorSplatY(VMVectorDivide(g_XMOne.v, d20)); // minimize div (reciprocal is not good enough)

	*dit1 = _mm_mul_ps(d20, delta_Y);
	*dit0 = _mm_mul_ps(d10, delta_Y);

	for (unsigned i = 1; i < Size / 16; i++)
	{
		dit1[i] = _mm_mul_ps(_mm_sub_ps(it2[i], it0[i]), delta_Y);
		dit0[i] = _mm_mul_ps(_mm_sub_ps(it1[i], it0[i]), delta_Y);
	}
	memcpy(_P, it0, Size);

	// call the flat triangle render routine, right edge interpolant is it0
	DrawFlatTriangle(self, it0, it2, dit0, dit1, _P);
}
void V_DrawTriangle(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2)
{
	if (v1->c.y < v0->c.y) swapptr(&v0, &v1);
	if (v2->c.y < v1->c.y) swapptr(&v1, &v2);
	if (v1->c.y < v0->c.y) swapptr(&v0, &v1);

	if (v0->c.y == v1->c.y)	// Flat Top
	{
		if (v1->c.x < v0->c.x) swapptr(&v0, &v1);
		_DrawFlatTopTriangle(self, v0, v1, v2);
	}
	else if (v1->c.y == v2->c.y)	// Flat Bottom
	{
		if (v2->c.x < v1->c.x) swapptr(&v1, &v2);
		_DrawFlatBottomTriangle(self, v0, v1, v2);
	}
	else // General
	{
		const float alphaSplit =
			(v1->c.y - v0->c.y) / (v2->c.y - v0->c.y);

		SVMVECTOR* vi = _alloca(self->VS.VSOutSize);
		VSOutInterpolate(vi, v0, v2, alphaSplit, self->VS.VSOutSize);

		if (v1->c.x < vi->c.x)
		{
			_DrawFlatBottomTriangle(self, (void*)v0, (void*)v1, (void*)vi);
			_DrawFlatTopTriangle(self, v1, vi, v2);
		}
		else
		{
			_DrawFlatBottomTriangle(self, (void*)v0, (void*)vi, (void*)v1);
			_DrawFlatTopTriangle(self, vi, v1, v2);
		}
	}
}

void _ProcessTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2)
{
	// perform homo -> ndc on xyz / perspective-correct interpolative divide on all other attributes
	VMVECTOR wInv = _mm_rcp_ps(_mm_shuffle_ps( _mm_unpackhi_ps(*v0, *v1), *v2, _MM_SHUFFLE(3, 3, 3, 2)));
	VMMATRIX X;
		X.r[0] = VMVectorSplatX( _mm_and_ps(wInv, g_XMMaskX.v));
		X.r[1] = VMVectorSplatY( _mm_and_ps(wInv, g_XMMaskY.v));
		X.r[2] = VMVectorSplatZ( _mm_and_ps(wInv, g_XMMaskZ.v));

	*v0 = VMVectorMultiplyAdd(_mm_mul_ps(*v0, X.r[0]), self->Global.Scale, self->Global.Offset); _mm_store_ss((float*)v0 + 3, X.r[0]);
	*v1 = VMVectorMultiplyAdd(_mm_mul_ps(*v1, X.r[1]), self->Global.Scale, self->Global.Offset); _mm_store_ss((float*)v1 + 3, X.r[1]);
	*v2 = VMVectorMultiplyAdd(_mm_mul_ps(*v2, X.r[2]), self->Global.Scale, self->Global.Offset); _mm_store_ss((float*)v2 + 3, X.r[2]);

	for (unsigned i = 1; i < self->VS.VSOutSize / 16; i++)
	{
		v0[i] = _mm_mul_ps(v0[i], X.r[0]);
		v1[i] = _mm_mul_ps(v1[i], X.r[1]);
		v2[i] = _mm_mul_ps(v2[i], X.r[2]);
	}

	if (self->GS)
		self->GS->Apply(self->GS, v0, v1, v2);
	V_DrawTriangle(self, (void*)v0, (void*)v1, (void*)v2);
}

void Clip1V(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2, size_t VSize)
{
	// calculate alpha values for getting adjusted vertices
	const float alphaA = (-v0->c.z) / (v1->c.z - v0->c.z);
	const float alphaB = (-v0->c.z) / (v2->c.z - v0->c.z);

	// interpolate to get v0a and v0b
	VMVECTOR* v0a1 = _alloca(VSize);
	VMVECTOR* v0a2 = _alloca(VSize);
	VMVECTOR* v0b = _alloca(VSize);
	VMVECTOR* v2b = _alloca(VSize);

	for (unsigned i = 0; i < VSize / 16; i++)
	{
		v0a1[i] = VMVectorLerp(v0[i].v, v1[i].v, alphaA);
		v0b[i] = VMVectorLerp(v0[i].v, v2[i].v, alphaB);
	}

	memcpy(v0a2, v0a1, VSize);
	memcpy(v2b, v2, VSize);

	// draw triangles
	_ProcessTriangle(self, v0a1, (void*)v1, (void*)v2);
	_ProcessTriangle(self, v0b, v0a2, v2b);
}
void Clip2V(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2, size_t VSize)
{
	// calculate alpha values for getting adjusted vertices
	const float alpha0 = (-v0->c.z) / (v2->c.z - v0->c.z);
	const float alpha1 = (-v1->c.z) / (v2->c.z - v1->c.z);
	// interpolate to get v0a and v0b
	VSOutInterpolate(v0, v0, v2, alpha0, VSize);
	VSOutInterpolate(v1, v1, v2, alpha1, VSize);
	// draw triangles
	_ProcessTriangle(self, (void*)v0, (void*)v1, (void*)v2);
};
void _ClipCullTriangle(selfptr, void* v0, void* v1, void* v2, size_t VSize)
{	
	const XMVECTORF32 VNegVector2 = { -1.0f,-1.0f, 0.0f, 1.0f };
	VMVECTOR V0 = _mm_load_ps(v0);
	VMVECTOR V1 = _mm_load_ps(v1);
	VMVECTOR V2 = _mm_load_ps(v2);

	// Compare againgst W value
	VMVECTOR CT0 = VMVectorSplatW(V0);
	VMVECTOR CT1 = VMVectorSplatW(V1);
	VMVECTOR CT2 = VMVectorSplatW(V2);

	VMVECTOR R01 = _mm_cmplt_ps(CT0, V0);
	VMVECTOR R11 = _mm_cmplt_ps(CT1, V1);
	VMVECTOR R21 = _mm_cmplt_ps(CT2, V2);

	VMVECTOR RR1 = _mm_and_ps(_mm_and_ps(R01, R11), R21);
	if (_mm_movemask_ps(RR1) != 0) return;

	CT0 = _mm_mul_ps(CT0, VNegVector2.v);
	CT1 = _mm_mul_ps(CT1, VNegVector2.v);
	CT2 = _mm_mul_ps(CT2, VNegVector2.v);

	VMVECTOR R02 = _mm_cmplt_ps(V0, CT0);
	VMVECTOR R12 = _mm_cmplt_ps(V1, CT1);
	VMVECTOR R22 = _mm_cmplt_ps(V2, CT2);
	VMVECTOR RR2 = _mm_and_ps(_mm_and_ps(R02, R12), R22);
	if (_mm_movemask_ps(RR2) != 0) return;

	RR1 = _mm_unpackhi_ps(R02, R12);
	RR2 = _mm_shuffle_ps(RR1, R22, _MM_SHUFFLE(3, 2, 1, 0));
	int selector = _mm_movemask_ps(RR2);
	switch (selector)
	{
	case 0: _ProcessTriangle(self, v0, v1, v2); break;
	case 1: Clip1V(self, v0, v1, v2, VSize); break;
	case 2: Clip1V(self, v1, v0, v2, VSize); break;
	case 3: Clip2V(self, v0, v1, v2, VSize); break;
	case 4: Clip1V(self, v2, v0, v1, VSize); break;
	case 5: Clip2V(self, v0, v2, v1, VSize); break;
	case 6: Clip2V(self, v1, v2, v0, VSize); break;
	}	
}

void _AssembleTriangles(selfptr, const void* Verts, size_t VSize, const size_t* indices, size_t indN)
{
	FVMVECTOR eyepos = VMVector4Transform((XMVECTORF32){ 0.0f,0.0f,0.0f,1.0f }.v, self->projection);

	VMVECTOR* Triangle = _alloca(VSize * 3);	//Opt triangle repr
	VMVECTOR* v0p = (Byte*)Triangle + 0 * VSize;
	VMVECTOR* v1p = (Byte*)Triangle + 1 * VSize;
	VMVECTOR* v2p = (Byte*)Triangle + 2 * VSize;

	for (size_t i = 0; i < indN; i += 3)
	{
		memcpy(v0p, ((Byte*)Verts + indices[i + 0] * VSize), VSize);
		memcpy(v1p, ((Byte*)Verts + indices[i + 1] * VSize), VSize);
		memcpy(v2p, ((Byte*)Verts + indices[i + 2] * VSize), VSize);

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
			_ClipCullTriangle(self, v0p, v1p, v2p, VSize);
		}
	}
}
void _ProcessVertices(selfptr, IndexedTriangleList* trilist)
{
	assert(self->VS.Apply);
	void* pVSOut = _aligned_malloc(self->VS.VSOutSize * trilist->numVerts, 16);

	if (pVSOut)
	{
		// Transform all the verts accordingly
		self->VS.Apply(&self->VS, pVSOut, trilist);
		_AssembleTriangles(self, pVSOut, self->VS.VSOutSize, trilist->indices, trilist->numInds);
	}
	_aligned_free(pVSOut);
}
void _Draw(selfptr, IndexedTriangleList* trilist)
{
	_ProcessVertices(self, trilist); // Start the pipeline
}

VirtualTable{
	.Draw = _Draw
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->gfx = va_arg(*ap, struct Frame*);

	const float HalfViewportWidth = self->gfx->nFrameLength * 0.5f;
	const float HalfViewportHeight = self->gfx->nFrameHeight * 0.5f;
	self->Global.Scale = VMVectorSet(HalfViewportWidth, -HalfViewportHeight, 1.0f, 0.0f);
	self->Global.Offset = VMVectorSet(HalfViewportWidth, HalfViewportHeight, 1.0f, 0.0f);

	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC