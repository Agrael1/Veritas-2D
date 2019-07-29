#include "GSBase.h"
#include "VSBase.h"
#include "PSBase.h"
#include "Standard.h"
#include "Pipeline.h"
#include "Class.h"

IAOut ia;

inline void DrawFlatTriangle(selfptr,
	FVMVECTOR* it0,
	FVMVECTOR* it1,
	FVMVECTOR* it2,
	FVMVECTOR* dv0,
	FVMVECTOR* dv1,
	VMVECTOR itEdge1,
	CHAR_INFO GSOut)
{
	account(self);
	
	// create edge interpolant for left edge (always v0)
	VMVECTOR itEdge0 = *it0;

	// calculate start and end scanlines
	const int yStart = max((int)ceilf(it0->m128_f32[1] - 0.5f), 0);
	const int yEnd = min((int)ceilf(it2->m128_f32[1] - 0.5f), (int)this->gfx->nFrameHeight - 1); // the scanline AFTER the last line drawn

	// do interpolant prestep
	const float step = ((float)yStart + 0.5f - it0->m128_f32[1]);
	itEdge0 = VMVectorAdd( itEdge0, VMVectorScale(*dv0, step));
	itEdge1 = VMVectorAdd(itEdge1, VMVectorScale(*dv1, step));

	for (int y = yStart; y < yEnd; y++,
		itEdge0 = VMVectorAdd(itEdge0, *dv0), 
		itEdge1 = VMVectorAdd(itEdge1, *dv1))
	{
		// calculate start and end pixels
		const int xStart = max((int)ceilf(itEdge0.m128_f32[0] - 0.5f),0);
		const int xEnd = min((int)ceilf(itEdge1.m128_f32[0] - 0.5f), (int)this->gfx->nFrameLength-1); // the pixel AFTER the last pixel drawn

														  // create scanline interpolant startpoint
														  // (some waste for interpolating x,y,z, but makes life easier not having
														  //  to split them off, and z will be needed in the future anyways...)
		VMVECTOR iLine = itEdge0;

		//// calculate delta scanline interpolant / dx
		const float dx = itEdge1.m128_f32[0] - itEdge0.m128_f32[0];
		FVMVECTOR diLine = VMVectorScale( VMVectorSubtract(itEdge1, iLine), 1.0f / dx);

		// prestep scanline interpolant
		iLine = VMVectorAdd(iLine, VMVectorScale(diLine, ((float)xStart + 0.5f - itEdge0.m128_f32[0])));

		for (int x = xStart; x < xEnd; x++, 
			iLine = VMVectorAdd(iLine,diLine))
		{
			if (self->gfx->method->DepthTest(self->gfx, x, y, iLine.m128_f32[2]))
			{
				// invoke pixel shader with interpolated vertex attributes
				// and use result to set the pixel color on the screen
				this->gfx->method->PrintFrame(this->gfx, x, y, self->PS->Apply(self->PS, nullptr));
			}
		}
	}
}

inline void DrawFlatTopTriangle2(selfptr, FVMVECTOR* it0,
	FVMVECTOR* it1,
	FVMVECTOR* it2,
	CHAR_INFO GSOut)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	const float delta_y = 1.0f / (it2->m128_f32[1] - it0->m128_f32[1]);
	FVMVECTOR dit0 = VMVectorScale(VMVectorSubtract(*it2, *it0), delta_y);
	FVMVECTOR dit1 = VMVectorScale(VMVectorSubtract(*it2, *it1), delta_y);

	// call the flat triangle render routine, right edge interpolant is it1
	DrawFlatTriangle(self, it0, it1, it2, &dit0, &dit1, *it1, GSOut);
}
inline void DrawFlatBottomTriangle2(selfptr, FVMVECTOR* it0,
	FVMVECTOR* it1,
	FVMVECTOR* it2,
	CHAR_INFO GSOut)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	const float delta_y = 1.0f / (it2->m128_f32[1] - it0->m128_f32[1]);
	FVMVECTOR dit0 = VMVectorScale(VMVectorSubtract(*it1, *it0), delta_y);
	FVMVECTOR dit1 = VMVectorScale(VMVectorSubtract(*it2, *it0), delta_y);

	// call the flat triangle render routine, right edge interpolant is it0
	DrawFlatTriangle(self, it0, it1, it2, &dit0, &dit1, *it0, GSOut);
}
void _DrawTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, CHAR_INFO GSOut)
{
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);
	if (v2->m128_f32[1] < v1->m128_f32[1]) swapptr(&v1, &v2);
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);

	if (v0->m128_f32[1] == v1->m128_f32[1])	// Flat Top
	{
		if (v1->m128_f32[0] < v0->m128_f32[0]) swapptr(&v0, &v1);
		DrawFlatTopTriangle2(self, v0, v1, v2, GSOut);
	}
	if (v1->m128_f32[1] == v2->m128_f32[1])	// Flat Bottom
	{
		if (v2->m128_f32[0] < v1->m128_f32[0]) swapptr(&v1, &v2);
		DrawFlatBottomTriangle2(self, v0, v1, v2, GSOut);
	}
	else // General
	{
		const float alphaSplit =
			(v1->m128_f32[1] - v0->m128_f32[1]) / (v2->m128_f32[1] - v0->m128_f32[1]);

		FVMVECTOR vi = VMVectorLerp(*v0, *v2, alphaSplit);

		if (v1->m128_f32[0] < vi.m128_f32[0])
		{
			DrawFlatBottomTriangle2(self, v0, v1, &vi, GSOut);
			DrawFlatTopTriangle2(self, v1, &vi, v2, GSOut);
		}
		else
		{
			DrawFlatBottomTriangle2(self, v0, &vi, v1, GSOut);
			DrawFlatTopTriangle2(self, &vi, v1, v2, GSOut);
		}
	}
}

inline void Transform(const selfptr, SVMVECTOR* v)
{
	// perform homo -> ndc on xyz / perspective-correct interpolative divide on all other attributes
	const float wInv = 1.0f / v->c.w;
	v->v = VMVectorScale(v->v, wInv);
	// additional divide for mapped z because it must be interpolated
	// adjust position x,y from perspective normalized space
	// to screen dimension space after perspective divide
	v->c.x = (v->c.x + 1.0f) * self->gfx->nFrameLength*0.5f;
	v->c.y = (-v->c.y + 1.0f) * self->gfx->nFrameHeight*0.5f;
	// store 1/w in w (we will need the interpolated 1/w
	// so that we can recover the attributes after interp.)
	v->c.w = wInv;
}
void _ProcessTriangle(selfptr, void* v0, void* v1, void* v2)
{
	Transform(self, v0);
	Transform(self, v1);
	Transform(self, v2);
	
	if(self->GS)
		self->GS->Apply(self->GS, v0, v1, v2);
	_DrawTriangle(self, v0, v1, v2, (CHAR_INFO) { 0 });
}

// Clipper Module
inline void Clip1V(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, size_t VSize)
{
	// calculate alpha values for getting adjusted vertices
	const float alphaA = (-v0->m128_f32[2]) / (v1->m128_f32[2] - v0->m128_f32[2]);
	const float alphaB = (-v0->m128_f32[2]) / (v2->m128_f32[2] - v0->m128_f32[2]);
	// interpolate to get v0a and v0b
	void* v0a = _alloca(VSize);
	void* v0b = _alloca(VSize);
	if (VSize != sizeof(VMVECTOR))
	{
		memcpy_s(v0a, VSize, v0 + sizeof(VMVECTOR), VSize - sizeof(VMVECTOR));
		memcpy_s(v0b, VSize, v0 + sizeof(VMVECTOR), VSize - sizeof(VMVECTOR));
	}

	*(VMVECTOR*)v0a = VMVectorLerp(*v0, *v1, alphaA);
	*(VMVECTOR*)v0b = VMVectorLerp(*v0, *v2, alphaB);
	// draw triangles
	_ProcessTriangle(self, v0a, v1, v2);
	_ProcessTriangle(self, v0b, v0a, v2);
}
inline void Clip2V(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, size_t VSize)
{
	// calculate alpha values for getting adjusted vertices
	const float alpha0 = (-v0->m128_f32[2]) / (v2->m128_f32[2] - v0->m128_f32[2]);
	const float alpha1 = (-v1->m128_f32[2]) / (v2->m128_f32[2] - v1->m128_f32[2]);
	// interpolate to get v0a and v0b
	*v0 = VMVectorLerp(*v0, *v2, alpha0);
	*v1 = VMVectorLerp(*v1, *v2, alpha1);
	// draw triangles
	_ProcessTriangle(self, v0, v1, v2);
};
void _ClipCullTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, size_t VSize)
{
	{
		// cull tests
		if (v0->m128_f32[0] > v0->m128_f32[3] &&
			v1->m128_f32[0] > v1->m128_f32[3] &&
			v2->m128_f32[0] > v2->m128_f32[3])
		{
			return;
		}
		if (v0->m128_f32[0] < -v0->m128_f32[3] &&
			v1->m128_f32[0] < -v1->m128_f32[3] &&
			v2->m128_f32[0] < -v2->m128_f32[3])
		{
			return;
		}
		if (v0->m128_f32[1] > v0->m128_f32[3] &&
			v1->m128_f32[1] > v1->m128_f32[3] &&
			v2->m128_f32[1] > v2->m128_f32[3])
		{
			return;
		}
		if (v0->m128_f32[1] < -v0->m128_f32[3] &&
			v1->m128_f32[1] < -v1->m128_f32[3] &&
			v2->m128_f32[1] < -v2->m128_f32[3])
		{
			return;
		}
		if (v0->m128_f32[2] > v0->m128_f32[3] &&
			v1->m128_f32[2] > v1->m128_f32[3] &&
			v2->m128_f32[2] > v2->m128_f32[3])
		{
			return;
		}
		if (v0->m128_f32[2] < 0.0f &&
			v1->m128_f32[2] < 0.0f &&
			v2->m128_f32[2] < 0.0f)
		{
			return;
		}
	}
	// near clipping tests
	if (v0->m128_f32[2] < 0.0f)
	{
		if (v1->m128_f32[2] < 0.0f)
		{
			Clip2V(self, v0, v1, v2, VSize);
		}
		else if (v2->m128_f32[2] < 0.0f)
		{
			Clip2V(self, v0, v2, v1, VSize);
		}
		else
		{
			Clip1V(self, v0, v1, v2, VSize);
		}
	}
	else if (v1->m128_f32[2] < 0.0f)
	{
		if (v2->m128_f32[2] < 0.0f)
		{
			Clip2V(self, v1, v2, v0, VSize);
		}
		else
		{
			Clip1V(self, v1, v0, v2, VSize);
		}
	}
	else if (v2->m128_f32[2] < 0.0f)
	{
		Clip1V(self, v2, v0, v1, VSize);
	}
	else // no near clipping necessary
	{
		_ProcessTriangle(self,v0,v1,v2);
	}
}

void _AssembleTriangles(selfptr, const void* Verts, size_t VSize, const size_t* indices, size_t indN)
{
	for (size_t i = 0; i < indN; i+=3)
	{
		void* v0 = _alloca(VSize);
		void* v1 = _alloca(VSize);
		void* v2 = _alloca(VSize);

		memcpy_s(v0, VSize, ((Byte*)Verts + indices[i + 0] * VSize), VSize);
		memcpy_s(v1, VSize, ((Byte*)Verts + indices[i + 1] * VSize), VSize);
		memcpy_s(v2, VSize, ((Byte*)Verts + indices[i + 2] * VSize), VSize);

		VMVECTOR* v0p = (VMVECTOR*)v0;
		VMVECTOR* v1p = (VMVECTOR*)v1;
		VMVECTOR* v2p = (VMVECTOR*)v2;

		ia.SV_PrimID = i/3;
		ia.SV_PrimN = VMVector3Cross(VMVectorSubtract(*v2p, *v0p), VMVectorSubtract(*v1p, *v0p));

		VMVECTOR Normal = VMVector3Dot(ia.SV_PrimN, *v0p);
		// Culling by normal
		if (Normal.m128_f32[0] >= 0.0f)
		{
			_ProcessTriangle(self, v0, v1, v2);
		}
	}
}
void _ProcessVertices(selfptr, struct IndexedTriangleList* trilist)
{
	account(self);
	if (!this->VS)
	{
		_AssembleTriangles(self, trilist->vertices, trilist->VSize, trilist->indices, trilist->numInds);
		return;
	}
		
	size_t memstore = trilist->VSize * trilist->numVerts;
	void* VertsOut = _malloca(memstore);			// local allocation on stack

	if (VertsOut)
	{
		// Transform all the verts accordingly
		this->VS->Apply(this->VS, VertsOut, trilist);
		_AssembleTriangles(self, VertsOut, trilist->VSize, trilist->indices, trilist->numInds);
	}
	_freea(VertsOut);
}

// Future thread ops, now the const call
void _Draw(selfptr, struct IndexedTriangleList* trilist)
{
	_ProcessVertices(self, trilist); // Start the pipeline
}
constructMethodTable(
	.Draw = _Draw
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