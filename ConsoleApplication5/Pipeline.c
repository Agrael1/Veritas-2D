#include "ColorIndexPS.h"
#include "Standard.h"
#include "Pipeline.h"
#include "Class.h"

inline void DrawFlatTriangle(selfptr,
	FVMVECTOR* it0,
	FVMVECTOR* it1,
	FVMVECTOR* it2,
	FVMVECTOR* dv0,
	FVMVECTOR* dv1,
	VMVECTOR itEdge1,
	size_t PrimID)
{
	account(self);
	CHAR_INFO ret = self->PS->method->Apply(self->PS, PrimID);
	
	// create edge interpolant for left edge (always v0)
	VMVECTOR itEdge0 = *it0;

	// calculate start and end scanlines
	const Word yStart = max((Word)ceilf(it0->m128_f32[1] - 0.5f), 0);
	const Word yEnd = min((Word)ceilf(it2->m128_f32[1] - 0.5f), this->gfx->nFrameHeight - 1); // the scanline AFTER the last line drawn

	// do interpolant prestep
	const float step = ((float)yStart + 0.5f - it0->m128_f32[1]);
	itEdge0 = VMVectorAdd( itEdge0, VMVectorScale(*dv0, step));
	itEdge1 = VMVectorAdd(itEdge1, VMVectorScale(*dv1, step));

	for (Word y = yStart; y < yEnd; y++,
		itEdge0 = VMVectorAdd(itEdge0, *dv0), 
		itEdge1 = VMVectorAdd(itEdge1, *dv1))
	{
		// calculate start and end pixels
		const Word xStart = (Word)ceilf(itEdge0.m128_f32[0] - 0.5f);
		const Word xEnd = (Word)ceilf(itEdge1.m128_f32[0] - 0.5f); // the pixel AFTER the last pixel drawn

														  // create scanline interpolant startpoint
														  // (some waste for interpolating x,y,z, but makes life easier not having
														  //  to split them off, and z will be needed in the future anyways...)
		VMVECTOR iLine = itEdge0;

		// calculate delta scanline interpolant / dx
		const float dx = 1.0f / itEdge1.m128_f32[0] - itEdge0.m128_f32[0];
		const float rdx = ((float)xStart + 0.5f - itEdge0.m128_f32[0]) * dx;

		FVMVECTOR diLine = VMVectorScale(VMVectorSubtract(itEdge1, iLine), dx);

		// prestep scanline interpolant
		iLine = VMVectorLerp(iLine, itEdge1, rdx);

		for (int x = xStart; x < xEnd; x++, 
			iLine = VMVectorAdd(iLine,diLine))
		{
			// recover interpolated z from interpolated 1/z
			const float z = 1.0f / iLine.m128_f32[2];
			// do z rejection / update of z buffer
			// skip shading step if z rejected (early z)
			if (self->gfx->method->DepthTest(self->gfx, x, y, z))
			{
				// recover interpolated attributes
				// (wasted effort in multiplying pos (x,y,z) here, but
				//  not a huge deal, not worth the code complication to fix)
				//const auto attr = iLine * z;
				// invoke pixel shader with interpolated vertex attributes
				// and use result to set the pixel color on the screen
				this->gfx->method->PrintFrame(this->gfx, x, y, ret);
			}
		}
	}
}

inline void DrawFlatTopTriangle2(selfptr, FVMVECTOR* it0,
	FVMVECTOR* it1,
	FVMVECTOR* it2,
	size_t PrimID)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	const float delta_y = 1.0f / (it2->m128_f32[1] - it0->m128_f32[1]);
	FVMVECTOR dit0 = VMVectorScale(VMVectorSubtract(*it2, *it0), delta_y);
	FVMVECTOR dit1 = VMVectorScale(VMVectorSubtract(*it2, *it1), delta_y);

	// call the flat triangle render routine, right edge interpolant is it1
	DrawFlatTriangle(self, it0, it1, it2, &dit0, &dit1, *it1, PrimID);
}
// does flat *BOTTOM* tri-specific calculations and calls DrawFlatTriangle
inline void DrawFlatBottomTriangle2(selfptr, FVMVECTOR* it0,
	FVMVECTOR* it1,
	FVMVECTOR* it2,
	size_t PrimID)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	const float delta_y = 1.0f / (it2->m128_f32[1] - it0->m128_f32[1]);
	FVMVECTOR dit0 = VMVectorScale(VMVectorSubtract(*it1, *it0), delta_y);
	FVMVECTOR dit1 = VMVectorScale(VMVectorSubtract(*it2, *it0), delta_y);

	// call the flat triangle render routine, right edge interpolant is it0
	DrawFlatTriangle(self, it0, it1, it2, &dit0, &dit1, *it0, PrimID);
}

void _DrawTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, size_t PrimID)
{
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);
	if (v2->m128_f32[1] < v1->m128_f32[1]) swapptr(&v1, &v2);
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);

	if (v0->m128_f32[1] == v1->m128_f32[1])	// Flat Top
	{
		if (v1->m128_f32[0] < v0->m128_f32[0]) swapptr(&v0, &v1);
		DrawFlatTopTriangle2(self, v0, v1, v2, PrimID);
	}
	if (v1->m128_f32[1] == v2->m128_f32[1])	// Flat Bottom
	{
		if (v2->m128_f32[0] < v1->m128_f32[0]) swapptr(&v1, &v2);
		DrawFlatBottomTriangle2(self, v0, v1, v2, PrimID);
	}
	else // General
	{
		const float alphaSplit =
			(v1->m128_f32[1] - v0->m128_f32[1]) / (v2->m128_f32[1] - v0->m128_f32[1]);

		FVMVECTOR vi = VMVectorLerp(*v0, *v2, alphaSplit);

		if (v1->m128_f32[0] < vi.m128_f32[0])
		{
			DrawFlatBottomTriangle2(self, v0, v1, &vi, PrimID);
			DrawFlatTopTriangle2(self, v1, &vi, v2, PrimID);
		}
		else
		{
			DrawFlatBottomTriangle2(self, v0, &vi, v1, PrimID);
			DrawFlatTopTriangle2(self, &vi, v1, v2, PrimID);
		}
	}
}
void _ProcessTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, size_t primID)
{
	*v0 = VMVector3Project(*v0, 0.0f, 0.0f, self->gfx->nFrameLength, self->gfx->nFrameHeight, 0.0f, 1.0f, self->gfx->world, &self->gfx->projection, &self->gfx->world);
	*v1 = VMVector3Project(*v1, 0.0f, 0.0f, self->gfx->nFrameLength, self->gfx->nFrameHeight, 0.0f, 1.0f, self->gfx->world, &self->gfx->projection, &self->gfx->world);
	*v2 = VMVector3Project(*v2, 0.0f, 0.0f, self->gfx->nFrameLength, self->gfx->nFrameHeight, 0.0f, 1.0f, self->gfx->world, &self->gfx->projection, &self->gfx->world);
	_DrawTriangle(self, v0, v1, v2, primID);
}
void _AssembleTriangles(selfptr, const VMVECTOR* Verts, const size_t* indices, size_t indN)
{
	for (size_t i = 0; i < indN; i+=3)
	{
		VMVECTOR v0 = Verts[indices[i]];
		VMVECTOR v1 = Verts[indices[i+1]];
		VMVECTOR v2 = Verts[indices[i+2]];

		VMVECTOR Normal = VMVector3Dot(VMVector3Cross(VMVectorSubtract(v2, v0), VMVectorSubtract(v1, v0)), v0);
		// Culling by normal
		if (Normal.m128_f32[0] >= 0.0f)
		{
			_ProcessTriangle(self, &v0, &v1, &v2, i);
		}
	}
}
void _ProcessVertices(selfptr, struct IndexedTriangleList* trilist)
{
	account(self);
	VMVECTOR* VertsOut = malloc(sizeof(VMVECTOR)*trilist->numVerts);

	// Transfor all the verts accordingly
	for (int i = 0; i < trilist->numVerts; i++)
	{ 
		VertsOut[i] = VMVector3TransformCoord(VMLoadFloat3(&trilist->vertices[i].pos), this->Transformation);
	}
	_AssembleTriangles(self, VertsOut, trilist->indices, trilist->numInds);
	free(VertsOut);
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
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC