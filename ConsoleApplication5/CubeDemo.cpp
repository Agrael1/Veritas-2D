#include "StringStream.h"
#include "VeritasMath.h"
#include "Class.h"
#include "Icosahedron.h"
#include "CubeDemo.h"
#include "Color.scheme"


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
		case VK_SPACE:
			this->bStop ^= true;
			break;
		case VK_TAB:
			this->cycle = (this->cycle + 1) % 3;
			break;
		case 'O':
			this->back = (this->back + 0x10) % 0x100;
			break;
		case 'P':
			this->fore = (this->fore + 0x1) % 18;
			break;
		}
	}
	return true;
}
bool virtual(OnUserCreate)(void* self)
{
	account(self);

	// using DB16 - DawnBringer's 16 Col Palette v1.0
	// http://pixeljoint.com/forum/forum_posts.asp?TID=12795
	COLORREF palette[16] = {
	RGB(20, 12, 28),		// Black
	RGB(68, 36, 52),		// Dark Magenta
	RGB(48, 52, 109),		// Dark Blue
	RGB(78, 74, 78),		// Grey
	RGB(133, 76, 48),		// Light Brown
	RGB(52, 101, 36),		// Grass Green
	RGB(208, 70, 72),		// Red
	RGB(117, 113, 97),		// Ditry Gray
	RGB(89, 125, 206),		// Blue
	RGB(10, 125, 44),		// Light Green
	RGB(133, 149, 161),		// Metal
	RGB(109, 170, 44),		// Acid Green
	RGB(210, 170, 153),		// Skin
	RGB(109, 194, 202),		// Sky
	RGB(218, 212, 94),		// Honey
	RGB(222, 238, 214)		// Moon White
	};

	base.Window->method->SetPalette(base.Window, palette);

	this->bStop = false;
	this->cycle = this->back = this->fore = 0;
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

	base.Output->method->ClearFrame(base.Output, ' ', BG_Sky);

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
				' ', BG_Dark_Magenta);
		
			base.Output->method->DrawTriangleWireframe(base.Output,
				(Word)v0.m128_f32[0], (Word)v0.m128_f32[1],
				(Word)v1.m128_f32[0], (Word)v1.m128_f32[1],
				(Word)v2.m128_f32[0], (Word)v2.m128_f32[1],
				' ', BG_Ditry_Gray);
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