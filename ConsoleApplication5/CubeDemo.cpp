#include "VActor.h"
#include "CubeTexDr.h"
#include "PhysicsAggregate.h"
#include "Drawable.h"
#include "Codex.h"
#include "Class.h"
#include "CubeDemo.h"
#include "Color.scheme"

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
		case VK_INSERT:
		{
			if (base.Control->bCursorEnabled)
			{
				base.Control->method->BlockCursor(base.Control);
			}
			else
			{
				base.Control->method->ReleaseCursor(base.Control);
			}
		}
		}
	}
	return true;
}
void virtual(HandleControls)(void* self, const struct Keyboard* kbd, double fElapsedTime)
{
	account(self);
	if (kbd->method->KeyPressed(kbd, 'W'))
	{
		this->actor->method->Move(this->actor, (VMFLOAT3A) { 0.0f, 0.0f, (float)fElapsedTime });
	}
	if (kbd->method->KeyPressed(kbd, 'S'))
	{
		this->actor->method->Move(this->actor, (VMFLOAT3A) { 0.0f, 0.0f, -(float)fElapsedTime });
	}
	if (kbd->method->KeyPressed(kbd, 'A'))
	{
		this->actor->method->Move(this->actor, (VMFLOAT3A) { -(float)fElapsedTime, 0.0f, 0.0f, });
	}
	if (kbd->method->KeyPressed(kbd, 'D'))
	{
		this->actor->method->Move(this->actor, (VMFLOAT3A) { (float)fElapsedTime, 0.0f, 0.0f });
	}
}
void virtual(HandleMouse)(void* self, struct Mouse* mouse, double fElapsedTime)
{
	account(self);
	if (!base.Control->bCursorEnabled)
	{
		int X; int Y;
		mouse->method->ReadMouseMovement(mouse, &X, &Y);
		float dX = (float)X*(float)fElapsedTime;
		float dY = (float)Y*(float)fElapsedTime;
		this->pActiveCamera->method->Rotate(this->pActiveCamera, dX, dY);
		if (mouse->WheelDelta != 0)
			this->pActiveCamera->method->Translate(this->pActiveCamera,
			(VMFLOAT3A) {
			0.0f, 0.0f, (float)fElapsedTime*(float)mouse->method->ReadWheelDelta(mouse)
		});
	}
}
bool virtual(OnUserCreate)(void* self)
{
	account(self);
	InitializeCodex();
	this->pPl = new(VLine, base.Output);
	this->pLight = new(DirectionalLight);

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
	this->actor = new(VActor);
	this->mesh = new(CubeDr);
	this->physics = new(Physics);

	this->pActiveCamera = this->actor->ACamera;

	this->pPl->projection = VMMatrixPerspectiveLH(1.0f, (float)base.Output->nFrameHeight / (float)base.Output->nFrameLength, 0.5f, 40.0f);
	this->pLight->_base.Bind(this->pLight, this->pPl);
	return true;
}
bool virtual(OnUserUpdate)(void* self, double fElapsedSeconds)
{
	account(self);

	if (this->bStop)
		return true;

	this->physics->method->Tick(this->physics);


	this->pPl->camera = this->pActiveCamera->method->GetViewMatrix(this->pActiveCamera);
	base.Output->method->BeginFrame(base.Output, ' ', BG_Sky);

	this->actor->Mesh->_base.method->Draw(this->actor->Mesh, this->pPl);
	this->physics->pMesh->method->Draw(this->physics->pMesh, this->pPl);

	return true;
}
bool virtual(OnUserDestroy)(void* self)
{
	account(self);
	
	delete_s(this->mesh);
	delete_s(this->actor);
	delete_s(this->pPl);

	DestroyCodex();
	return true;
}

Constructor(void* self, va_list *ap)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->ctor(self, ap);
	base.AppName = stringOf(CubeDemo);
	base.method->OnUserCreate = virtual(OnUserCreate);
	base.method->HandleControls = virtual(HandleControls);
	base.method->OnUserUpdate = virtual(OnUserUpdate);
	base.method->HandleInputEvents = virtual(HandleInputEvents);
	base.method->OnUserDestroy = virtual(OnUserDestroy);
	base.method->HandleMouse = virtual(HandleMouse);
	return this;
}
Destructor(void* self)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->dtor(self);
	return this;
}
ENDCLASSDESC