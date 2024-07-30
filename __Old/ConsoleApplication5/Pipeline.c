//#include "GSBase.h"
//#include "VSBase.h"
//#include "PSBase.h"
//#include "Color.h"
//#include "Standard.h"
//#include "Pipeline.h"
//#include "Class.h"
//
//IAOut ia;
//
//inline void DrawFlatTriangle(selfptr,
//	SVMVECTOR* it0,
//	SVMVECTOR* it1,
//	SVMVECTOR* it2,
//	FVMVECTOR* dv0,
//	FVMVECTOR* dv1,
//	VMVECTOR itEdge1)
//{
//	account(self);
//	
//	// create edge interpolant for left edge (always v0)
//	VMVECTOR itEdge0 = it0->v;
//
//	// calculate start and end scanlines
//	const int yStart = max((int)ceilf(it0->c.y - 0.5f), 0);
//	const int yEnd = min((int)ceilf(it2->c.y - 0.5f), (int)this->gfx->nFrameHeight - 1); // the scanline AFTER the last line drawn
//
//	// do interpolant prestep
//	const float step = ((float)yStart + 0.5f - it0->c.y);
//	itEdge0 = VMVectorAdd(itEdge0, VMVectorScale(*dv0, step));
//	itEdge1 = VMVectorAdd(itEdge1, VMVectorScale(*dv1, step));
//
//	for (int y = yStart; y < yEnd; y++,
//		itEdge0 = VMVectorAdd(itEdge0, *dv0), 
//		itEdge1 = VMVectorAdd(itEdge1, *dv1))
//	{
//		// calculate start and end pixels
//		const int xStart = max((int)ceilf(itEdge0.m128_f32[0] - 0.5f),0);
//		const int xEnd = min((int)ceilf(itEdge1.m128_f32[0] - 0.5f), (int)this->gfx->nFrameLength-1); // the pixel AFTER the last pixel drawn
//
//														  // create scanline interpolant startpoint
//														  // (some waste for interpolating x,y,z, but makes life easier not having
//														  //  to split them off, and z will be needed in the future anyways...)
//		SVMVECTOR iLine = { .v = itEdge0 };
//
//		//// calculate delta scanline interpolant / dx
//		const float dx = itEdge1.m128_f32[0] - itEdge0.m128_f32[0];
//		FVMVECTOR diLine = VMVectorScale( VMVectorSubtract(itEdge1, iLine.v), 1.0f / dx);
//
//		// prestep scanline interpolant
//		iLine.v = VMVectorAdd(iLine.v, VMVectorScale(diLine, ((float)xStart + 0.5f - itEdge0.m128_f32[0])));
//
//		for (int x = xStart; x < xEnd; x++, 
//			iLine.v = VMVectorAdd(iLine.v,diLine))
//		{
//			if (self->gfx->method->DepthTest(self->gfx, x, y, iLine.c.z))
//			{
//				// invoke pixel shader with interpolated vertex attributes
//				// and use result to set the pixel color on the screen
//				this->gfx->method->PrintFrame(this->gfx, x, y, self->PS->Apply(self->PS, nullptr));
//			}
//		}
//	}
//}
//inline void DrawFlatTopTriangle2(selfptr, SVMVECTOR* it0,
//	SVMVECTOR* it1,
//	SVMVECTOR* it2)
//{
//	// calulcate dVertex / dy
//	// change in interpolant for every 1 change in y
//	const float delta_y = 1.0f / (it2->c.y - it0->c.y);
//	FVMVECTOR dit0 = VMVectorScale(VMVectorSubtract(it2->v, it0->v), delta_y);
//	FVMVECTOR dit1 = VMVectorScale(VMVectorSubtract(it2->v, it1->v), delta_y);
//
//	// call the flat triangle render routine, right edge interpolant is it1
//	DrawFlatTriangle(self, it0, it1, it2, &dit0, &dit1, it1->v);
//}
//inline void DrawFlatBottomTriangle2(selfptr, SVMVECTOR* it0,
//	SVMVECTOR* it1,
//	SVMVECTOR* it2)
//{
//	// calulcate dVertex / dy
//	// change in interpolant for every 1 change in y
//	const float delta_y = 1.0f / (it2->c.y - it0->c.y);
//	FVMVECTOR dit0 = VMVectorScale(VMVectorSubtract(it1->v, it0->v), delta_y);
//	FVMVECTOR dit1 = VMVectorScale(VMVectorSubtract(it2->v, it0->v), delta_y);
//
//	// call the flat triangle render routine, right edge interpolant is it0
//	DrawFlatTriangle(self, it0, it1, it2, &dit0, &dit1, it0->v);
//}
//void _DrawTriangle(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2)
//{
//	if (v1->c.y < v0->c.y) swapptr(&v0, &v1);
//	if (v2->c.y < v1->c.y) swapptr(&v1, &v2);
//	if (v1->c.y < v0->c.y) swapptr(&v0, &v1);
//
//	if (v0->c.y == v1->c.y)	// Flat Top
//	{
//		if (v1->c.x < v0->c.x) swapptr(&v0, &v1);
//		DrawFlatTopTriangle2(self, v0, v1, v2);
//	}
//	if (v1->c.y == v2->c.y)	// Flat Bottom
//	{
//		if (v2->c.x < v1->c.x) swapptr(&v1, &v2);
//		DrawFlatBottomTriangle2(self, v0, v1, v2);
//	}
//	else // General
//	{
//		const float alphaSplit =
//			(v1->c.y - v0->c.y) / (v2->c.y - v0->c.y);
//
//		SVMVECTOR vi = {.v = VMVectorLerp(v0->v, v2->v, alphaSplit) };
//
//		if (v1->c.x < vi.c.x)
//		{
//			DrawFlatBottomTriangle2(self, v0, v1, &vi);
//			DrawFlatTopTriangle2(self, v1, &vi, v2);
//		}
//		else
//		{
//			DrawFlatBottomTriangle2(self, v0, &vi, v1);
//			DrawFlatTopTriangle2(self, &vi, v1, v2);
//		}
//	}
//}
//
//inline void Transform(const selfptr, SVMVECTOR* v)
//{
//	// perform homo -> ndc on xyz / perspective-correct interpolative divide on all other attributes
//	const float wInv = 1.0f / v->c.w;
//	v->v = VMVectorScale(v->v, wInv);
//
//	const float HalfViewportWidth = self->gfx->nFrameLength * 0.5f;
//	const float HalfViewportHeight = self->gfx->nFrameHeight * 0.5f;
//
//	SVMVECTOR Scale = { HalfViewportWidth, -HalfViewportHeight, 1.0f, 0.0f };
//	SVMVECTOR Offset = { HalfViewportWidth, HalfViewportHeight, 1.0f, 0.0f };
//	v->v = VMVectorMultiplyAdd(v->v, Scale.v, Offset.v);
//	v->c.w = wInv;
//}
//void _ProcessTriangle(selfptr, void* v0, void* v1, void* v2)
//{
//	Transform(self, v0);
//	Transform(self, v1);
//	Transform(self, v2);
//	
//	if(self->GS)
//		self->GS->Apply(self->GS, v0, v1, v2);
//	_DrawTriangle(self, v0, v1, v2);
//}
//
////Clipper Module
//inline void Clip1V(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2, size_t VSize)
//{
//	// calculate alpha values for getting adjusted vertices
//	const float alphaA = (2.0f-v0->c.z) / (v1->c.z - v0->c.z);
//	const float alphaB = (2.0f-v0->c.z) / (v2->c.z - v0->c.z);
//	// interpolate to get v0a and v0b
//	VMVECTOR* v0a1 = _alloca(VSize);
//	VMVECTOR* v0a2 = _alloca(VSize);
//	VMVECTOR* v0b = _alloca(VSize);
//	VMVECTOR* v2b = _alloca(VSize);
//	if (VSize != sizeof(VMVECTOR))
//	{
//		memcpy_s(v0a1, VSize, v0 + sizeof(VMVECTOR), VSize - sizeof(VMVECTOR));
//		memcpy_s(v0a2, VSize, v0 + sizeof(VMVECTOR), VSize - sizeof(VMVECTOR));
//		memcpy_s(v0b, VSize, v0 + sizeof(VMVECTOR), VSize - sizeof(VMVECTOR));
//		memcpy_s(v2b, VSize, v2 , VSize);
//	}
//
//	*v0a1 = VMVectorLerp(v0->v, v1->v, alphaA);
//	*v0a2 = *v0a1;
//	*v0b = VMVectorLerp(v0->v, v2->v, alphaB);
//	// draw triangles
//	_ProcessTriangle(self, v0a1, v1, v2);
//	_ProcessTriangle(self, v0b, v0a2, v2b);
//}
//inline void Clip2V(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2)
//{
//	// calculate alpha values for getting adjusted vertices
//	const float alpha0 = (2.0f - v0->c.z) / (v2->c.z - v0->c.z);
//	const float alpha1 = (2.0f - v1->c.z) / (v2->c.z - v1->c.z);
//	// interpolate to get v0a and v0b
//	v0->v = VMVectorLerp(v0->v, v2->v, alpha0);
//	v1->v = VMVectorLerp(v1->v, v2->v, alpha1);
//	// draw triangles
//	_ProcessTriangle(self, v0, v1, v2);
//};
//void _ClipCullTriangle(selfptr, SVMVECTOR* v0, SVMVECTOR* v1, SVMVECTOR* v2, size_t VSize)
//{
//	{
//		// cull tests
//		if (v0->c.x > v0->c.w &&
//			v1->c.x > v1->c.w &&
//			v2->c.x > v2->c.w)
//		{
//			return;
//		}
//		if (v0->c.x < -v0->c.w &&
//			v1->c.x < -v1->c.w &&
//			v2->c.x < -v2->c.w)
//		{
//			return;
//		}
//		if (v0->c.y > v0->c.w &&
//			v1->c.y > v1->c.w &&
//			v2->c.y > v2->c.w)
//		{
//			return;
//		}
//		if (v0->c.y < -v0->c.w &&
//			v1->c.y < -v1->c.w &&
//			v2->c.y < -v2->c.w)
//		{
//			return;
//		}
//		if (v0->c.z > v0->c.w &&
//			v1->c.z > v1->c.w &&
//			v2->c.z > v2->c.w)
//		{
//			return;
//		}
//		if (v0->c.z < 2.0f &&
//			v1->c.z < 2.0f &&
//			v2->c.z < 2.0f)
//		{
//			return;
//		}
//	}
//	// near clipping tests
//	if (v0->c.z < 2.0f)
//	{
//		if (v1->c.z < 2.0f)
//		{
//			Clip2V(self, v0, v1, v2);
//		}
//		else if (v2->c.z < 2.0f)
//		{
//			Clip2V(self, v0, v2, v1);
//		}
//		else
//		{
//			Clip1V(self, v0, v1, v2, VSize);
//		}
//	}
//	else if (v1->c.z < 2.0f)
//	{
//		if (v2->c.z < 2.0f)
//		{
//			Clip2V(self, v1, v2, v0);
//		}
//		else
//		{
//			Clip1V(self, v1, v0, v2, VSize);
//		}
//	}
//	else if (v2->c.z < 2.0f)
//	{
//		Clip1V(self, v2, v0, v1, VSize);
//	}
//	else // no near clipping necessary
//	{
//		_ProcessTriangle(self,v0,v1,v2);
//	}
//}
//
//void _AssembleTriangles(selfptr, const void* Verts, size_t VSize, const size_t* indices, size_t indN)
//{
//	void* v0 = _alloca(VSize);
//	void* v1 = _alloca(VSize);
//	void* v2 = _alloca(VSize);
//
//	for (size_t i = 0; i < indN; i+=3)
//	{
//		memcpy_s(v0, VSize, ((Byte*)Verts + indices[i + 0] * VSize), VSize);
//		memcpy_s(v1, VSize, ((Byte*)Verts + indices[i + 1] * VSize), VSize);
//		memcpy_s(v2, VSize, ((Byte*)Verts + indices[i + 2] * VSize), VSize);
//
//		VMVECTOR* v0p = (VMVECTOR*)v0;
//		VMVECTOR* v1p = (VMVECTOR*)v1;
//		VMVECTOR* v2p = (VMVECTOR*)v2;
//
//		//ia.SV_PrimID = i/3;
//		SVMVECTOR Normal = { .v = VMVector3Dot(
//			VMVector3Cross
//			(
//				VMVectorSubtract(*v1p, *v0p),
//				VMVectorSubtract(*v2p, *v0p)
//			), *v0p) };
//
//		// Culling by normal
//		if (Normal.v.m128_f32[0] <= 0.0f)
//		{
//			_ClipCullTriangle(self, v0, v1, v2, VSize);
//		}
//	}
//}
//void _ProcessVertices(selfptr, struct IndexedTriangleList* trilist)
//{
//	assert(self->VS);
//	void* VertsOut = _malloca(trilist->VSize * trilist->numVerts);			// local allocation on stack
//
//	if (VertsOut)
//	{
//		// Transform all the verts accordingly
//		self->VS->Apply(self->VS, VertsOut, trilist);
//		_AssembleTriangles(self, VertsOut, trilist->VSize, trilist->indices, trilist->numInds);
//	}
//	_freea(VertsOut);
//}
//
//// Future thread ops, now the const call
//void _Draw(selfptr, struct IndexedTriangleList* trilist)
//{
//	_ProcessVertices(self, trilist); // Start the pipeline
//}
//constructMethodTable(
//	.Draw = _Draw
//);
//
//Constructor(selfptr, va_list *ap)
//{
//	InitializeVtable(self);
//	self->gfx = va_arg(*ap, struct Frame*);
//	return self;
//}
//Destructor(selfptr)
//{
//	return self;
//}
//ENDCLASSDESC