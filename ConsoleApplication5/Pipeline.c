#include "ExperimentalPS.h"
#include "Standard.h"
#include "Pipeline.h"
#include "Class.h"

inline void DrawFlatTopTriangle(selfptr, const VMVECTOR* v0, const VMVECTOR* v1, const VMVECTOR* v2, size_t PrimID)
{
	// calculate slopes in screen space
	VMVECTOR res = VMVectorSubtract(*v2, *v0);
	const float m0 = res.m128_f32[0] / res.m128_f32[1];
	res = VMVectorSubtract(*v2, *v1);
	const float m1 = res.m128_f32[0] / res.m128_f32[1];

	struct PSOut ret = self->PS->method->Apply(self->PS, PrimID);

	// calculate start and end scanlines
	const int yStart = (int)ceilf(v0->m128_f32[1] - 0.5f);
	const int yEnd = (int)ceilf(v2->m128_f32[1] - 0.5f);

	for (int y = yStart; y < yEnd; y++)
	{
		// calculate start and end points(x coord)
		const float px0 = m0*((float)y + 0.5f - v0->m128_f32[1]) + v0->m128_f32[0];
		const float px1 = m1*((float)y + 0.5f - v1->m128_f32[1]) + v1->m128_f32[0];

		// start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f);

		for (int x = xStart; x < xEnd; x++)
		{
			self->gfx->method->PrintFrame(self->gfx, x, y, ret.sym, ret.color);
		}
	}
}
inline void DrawFlatBottomTriangle(selfptr, const VMVECTOR* v0, const VMVECTOR* v1, const VMVECTOR* v2, size_t PrimID)
{
	// calculate slopes in screen space
	VMVECTOR res = VMVectorSubtract(*v1, *v0);
	const float m0 = res.m128_f32[0] / res.m128_f32[1];
	res = VMVectorSubtract(*v2, *v0);
	const float m1 = res.m128_f32[0] / res.m128_f32[1];

	struct PSOut ret = self->PS->method->Apply(self->PS, PrimID);

	// calculate start and end scanlines
	const int yStart = (int)ceilf(v0->m128_f32[1] - 0.5f);
	const int yEnd = (int)ceilf(v2->m128_f32[1] - 0.5f);

	for (int y = yStart; y < yEnd; y++)
	{
		// calculate start and end points(x coord)
		const float px0 = m0*((float)y + 0.5f - v0->m128_f32[1]) + v0->m128_f32[0];
		const float px1 = m1*((float)y + 0.5f - v0->m128_f32[1]) + v0->m128_f32[0];

		// start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f);

		for (int x = xStart; x < xEnd; x++)
		{
			
			self->gfx->method->PrintFrame(self->gfx, x, y, ret.sym, ret.color);
		}
	}
}
void _DrawTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, size_t PrimID)
{
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);
	if (v2->m128_f32[1] < v1->m128_f32[1]) swapptr(&v1, &v2);
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);

	if (v0->m128_f32[1] == v1->m128_f32[1])	// Flat Top
	{
		if (v1->m128_f32[0] < v0->m128_f32[0]) swapptr(&v0, &v1);
		DrawFlatTopTriangle(self, v0, v1, v2, PrimID);
	}
	if (v1->m128_f32[1] == v2->m128_f32[1])	// Flat Bottom
	{
		if (v2->m128_f32[0] < v1->m128_f32[0]) swapptr(&v1, &v2);
		DrawFlatBottomTriangle(self, v0, v1, v2, PrimID);
	}
	else // General
	{
		const float alphaSplit =
			(v1->m128_f32[1] - v0->m128_f32[1]) / (v2->m128_f32[1] - v0->m128_f32[1]);

		const VMVECTOR vi = VMVectorAdd(*v0, VMVectorScale(VMVectorSubtract(*v2, *v0), alphaSplit));

		if (v1->m128_f32[0] < vi.m128_f32[0])
		{
			DrawFlatBottomTriangle(self, v0, v1, &vi, PrimID);
			DrawFlatTopTriangle(self, v1, &vi, v2, PrimID);
		}
		else
		{
			DrawFlatBottomTriangle(self, v0, &vi, v1, PrimID);
			DrawFlatTopTriangle(self, &vi, v1, v2, PrimID);
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
	self->PS = new(ExperimentalPS);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC