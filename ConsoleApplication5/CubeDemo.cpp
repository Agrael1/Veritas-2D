#include "StringStream.h"
#include "VeritasMath.h"
#include "Class.h"
#include "Icosphere.h"
#include "CubeDemo.h"
#include "Color.scheme"
#include <stdlib.h>

float GetRand(float min, float max)
{
	return ((float)rand() / (float)(RAND_MAX)) * max - min;
}

bool virtual(HandleInputEvents)(void* self, const KeyboardEvent* event)
{
	account(self);
	if (event->type == Press)
	{
		// check if the event was for the space key
		switch (event->code)
		{
		case VK_ESCAPE:
			return false;
		case VK_SPACE:
			this->bStop ^= true;
			break;
		}
	}
	return true;
}
void virtual(HandleControls)(void* self, const struct Keyboard* kbd, double fElapsedTime)
{
	account(self);
	if (kbd->method->KeyPressed(kbd, 'W'))
	{
		if(this->pCam->r > 5.0f)
			this->pCam->r -= 8.0f*(float)fElapsedTime;
	}
	if (kbd->method->KeyPressed(kbd, 'S'))
	{
		if (this->pCam->r < 40.0f)
			this->pCam->r += 8.0f*(float)fElapsedTime;
	}
}
bool virtual(OnUserCreate)(void* self)
{
	account(self);
	this->pCam = new(Camera); // Create an instance of a camera
	this->pPl = new(Pipeline, base.Output);


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
	srand((unsigned int)time(NULL));
	
	this->model = new(Icosphere,
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f),
		GetRand(0.1f, 5.0f));
	
	this->pPl->VS = this->model->VS;
	// Setting up projection matrix and camera
	base.Output->projection = VMMatrixPerspectiveLH(1.0f, (float)base.Output->nFrameHeight / (float)base.Output->nFrameLength, 0.5f, 40.0f);
	base.Output->world = VMMatrixIdentity();

	return true;
}
bool virtual(OnUserUpdate)(void* self, double fElapsedSeconds)
{
	account(self);

	if (this->bStop)
		return true;

	base.Output->camera = this->pCam->method->GetViewMatrix(this->pCam);
	base.Output->method->BeginFrame(base.Output, ' ', BG_Sky);

	this->model->_base.method->Update(this->model, (float)fElapsedSeconds);

	this->model->VS->Transformation = VMMatrixMultiply(this->model->_base.method->GetTransformXM(this->model), &base.Output->camera);
	this->pPl->method->Draw(this->pPl, &this->model->model);
	return true;
}
bool virtual(OnUserDestroy)(void* self)
{
	account(self);
	if (this->model->model.indices)
		free(this->model->model.indices);
	if (this->model->model.vertices)
		free(this->model->model.vertices);
	if(this->pCam)
		delete(this->pCam);
	if (this->pPl)
		delete(this->pPl);
	return true;
}

Constructor(void* self, va_list *ap)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->ctor(self, ap);
	base.AppName = stringOf(self);
	base.method->OnUserCreate = virtual(OnUserCreate);
	base.method->HandleControls = virtual(HandleControls);
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