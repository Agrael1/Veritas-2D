#include "VeritasMath.h"
#include "Class.h"
#include "Icosahedron.h"
#include "CubeDemo.h"

bool virtual(OnUserCreate)(void* self)
{
	account(self);
	
	this->model = Icosahedron_Make();
	
	// Setting up projection matrix and camera
	base.Output->projection = VMMatrixPerspectiveLH(1.0f, (float)base.Output->nFrameHeight / (float)base.Output->nFrameLength, 0.5f, 40.0f);
	base.Output->camera = VMMatrixTranslation(0.0f, 0.0f, 5.0f);

	return true;
}
bool virtual(OnUserUpdate)(void* self, double fElapsedSeconds)
{
	account(self);
	base.Output->method->ClearFrame(base.Output, 0, BG_BLACK);

	this->fTheta += (float)fElapsedSeconds;

	// Calculate rotation matrix
	VMMATRIX Rotation = VMMatrixRotationRollPitchYaw(-this->fTheta, 0, this->fTheta);

	for (UINT i = 0; i < this->model.numInds; i+=3)
	{
		// Extract triangle
		VMVECTOR v0 = VMLoadFloat3(&this->model.vertices[this->model.indices[i]].pos);
		VMVECTOR v1 = VMLoadFloat3(&this->model.vertices[this->model.indices[i+1]].pos);
		VMVECTOR v2 = VMLoadFloat3(&this->model.vertices[this->model.indices[i+2]].pos);

		// Rotate and project to View Space
		VMMATRIX Project = VMMatrixMultiply(Rotation, &base.Output->camera);

		v0 = VMVector3TransformCoord(v0, Project);
		v1 = VMVector3TransformCoord(v1, Project);
		v2 = VMVector3TransformCoord(v2, Project);

		VMVECTOR Normal = VMVector3Dot(VMVector3Cross(VMVectorSubtract(v2, v0), VMVectorSubtract(v1, v0)), v0);

		// Do a culling test
		if (Normal.m128_f32[0] >= 0.0f)
		{
			// Do a transformation to view
			v0 = VMVector3TransformCoord(v0, base.Output->projection);
			v1 = VMVector3TransformCoord(v1, base.Output->projection);
			v2 = VMVector3TransformCoord(v2, base.Output->projection);

			// Scale into view
			v0.m128_f32[0] += 1.0f; v0.m128_f32[1] += 1.0f;
			v1.m128_f32[0] += 1.0f; v1.m128_f32[1] += 1.0f;
			v2.m128_f32[0] += 1.0f; v2.m128_f32[1] += 1.0f;

			v0.m128_f32[0] *= 0.5f * (float)base.Output->nFrameLength;
			v0.m128_f32[1] *= 0.5f * (float)base.Output->nFrameHeight;
			v1.m128_f32[0] *= 0.5f * (float)base.Output->nFrameLength;
			v1.m128_f32[1] *= 0.5f * (float)base.Output->nFrameHeight;
			v2.m128_f32[0] *= 0.5f * (float)base.Output->nFrameLength;
			v2.m128_f32[1] *= 0.5f * (float)base.Output->nFrameHeight;


			// Rasterize triangle
			base.Output->method->DrawTriangle(base.Output,
				(Word)v0.m128_f32[0], (Word)v0.m128_f32[1],
				(Word)v1.m128_f32[0], (Word)v1.m128_f32[1],
				(Word)v2.m128_f32[0], (Word)v2.m128_f32[1],
				0x2588, FG_WHITE);
		}	
	}
	return true;
}
bool virtual(OnUserDestroy)(void* self)
{
	account(self);
	if (this->model.indices)
		free(this->model.indices);
	if (this->model.vertices)
		free(this->model.vertices);
	return true;
}

Constructor(void* self, va_list *ap)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->ctor(self, ap);
	base.AppName = stringOf(self);
	base.method->OnUserCreate = virtual(OnUserCreate);
	base.method->OnUserUpdate = virtual(OnUserUpdate);
	base.method->OnUserDestroy = virtual(OnUserDestroy);
	return this;
}
Destructor(void* self)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->dtor(self);

	return this;
}
ENDCLASSDESC