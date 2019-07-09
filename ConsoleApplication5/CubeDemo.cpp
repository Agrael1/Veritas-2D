#include "VeritasMath.h"
#include "Class.h"
#include "Icosahedron.h"
#include "CubeDemo.h"


bool virtual(HandleInputEvents)(void* self, struct KeyboardEvent event)
{
	account(self);
	if (event.method->IsPress(&event))
	{
		// check if the event was for the space key
		switch (event.method->GetCode(&event))
		{
		case VK_ESCAPE:
			return false;
		case 'M':
			this->bStop ^= true;
			break;
		}
	}
	return true;
}
bool virtual(OnUserCreate)(void* self)
{
	account(self);
	this->bStop = false;
	this->model = Icosahedron_Make();
	
	// Setting up projection matrix and camera
	base.Output->projection = VMMatrixPerspectiveLH(1.0f, (float)base.Output->nFrameHeight / (float)base.Output->nFrameLength, 0.5f, 40.0f);
	base.Output->camera = VMMatrixTranslation(0.0f, 0.0f, 5.0f);

	// probably a temp solution
	this->ScreenOffset = g_XMOne3.v;
	this->ScreenResolution = (VMVECTOR) { 0.5f * (float)base.Output->nFrameLength, 0.5f * (float)base.Output->nFrameHeight, 1.0f, 1.0f };

	return true;
}
bool virtual(OnUserUpdate)(void* self, double fElapsedSeconds)
{
	account(self);

	if (this->bStop)
		return true;

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
			v0 = VMVectorMultiply(VMVectorAdd(v0, this->ScreenOffset), this->ScreenResolution);
			v1 = VMVectorMultiply(VMVectorAdd(v1, this->ScreenOffset), this->ScreenResolution);
			v2 = VMVectorMultiply(VMVectorAdd(v2, this->ScreenOffset), this->ScreenResolution);

			base.Output->method->DrawTriangle(base.Output,
				&v0,
				&v1,
				&v2,
				0x2588, FG_WHITE);

			base.Output->method->DrawTriangleWireframe(base.Output,
				(Word)v0.m128_f32[0], (Word)v0.m128_f32[1],
				(Word)v1.m128_f32[0], (Word)v1.m128_f32[1],
				(Word)v2.m128_f32[0], (Word)v2.m128_f32[1],
				0x2588, FG_BLUE);
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
	base.method->HandleInputEvents = virtual(HandleInputEvents);
	base.method->OnUserDestroy = virtual(OnUserDestroy);
	return this;
}
Destructor(void* self)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->dtor(self);

	return this;
}
ENDCLASSDESC