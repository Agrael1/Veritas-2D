#include "VSBase.h"
#include "ColorIndexPS.h"
#include "FlatLightGS.h"
#include "Standard.h"
#include "Pipeline.h"
#include "Class.h"

typedef struct
{
	size_t SV_PrimID;
	VMVECTOR SV_PrimN;
}IAOut;

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
	CHAR_INFO ret = GSOut;
	
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
				this->gfx->method->PrintFrame(this->gfx, x, y, self->PS->method->Apply(self->PS, 1));
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
void _ProcessTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2)
{
	*v0 = VMVector3Project(*v0, 0.0f, 0.0f, self->gfx->nFrameLength, self->gfx->nFrameHeight, 0.0f, 1.0f, self->gfx->world, &self->gfx->projection, &self->gfx->world);
	*v1 = VMVector3Project(*v1, 0.0f, 0.0f, self->gfx->nFrameLength, self->gfx->nFrameHeight, 0.0f, 1.0f, self->gfx->world, &self->gfx->projection, &self->gfx->world);
	*v2 = VMVector3Project(*v2, 0.0f, 0.0f, self->gfx->nFrameLength, self->gfx->nFrameHeight, 0.0f, 1.0f, self->gfx->world, &self->gfx->projection, &self->gfx->world);
	_DrawTriangle(self, v0, v1, v2, self->GS->method->Apply(ia.SV_PrimN));
}
void _AssembleTriangles(selfptr, const VMVECTOR* Verts, size_t VSize, const size_t* indices, size_t indN)
{
	for (size_t i = 0; i < indN; i+=3)
	{
		VMVECTOR v0p = *(Verts + indices[i]);
		VMVECTOR v1p = *(Verts+indices[i+1]);
		VMVECTOR v2p = *(Verts+indices[i+2]);

		ia.SV_PrimID = i/3;
		ia.SV_PrimN = VMVector3Cross(VMVectorSubtract(v2p, v0p), VMVectorSubtract(v1p, v0p));

		VMVECTOR Normal = VMVector3Dot(ia.SV_PrimN, v0p);
		// Culling by normal
		if (Normal.m128_f32[0] >= 0.0f)
		{
			_ProcessTriangle(self, &v0p, &v1p, &v2p);
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
	self->PS = new(ColorIndexPS);
	self->GS = new(FlatLightGS);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC