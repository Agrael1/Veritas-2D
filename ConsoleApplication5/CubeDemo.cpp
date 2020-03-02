#include "VActor.h"
#include "CubeTexDr.h"
#include "PhysicsAggregate.h"
#include "Drawable.h"
#include "Model.h"
#include "Codex.h"

#include "TextureBuffer.h"

#include "Class.h"
#include "CubeDemo.h"
#include "L16TC.palette"

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
		case 'R':
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
	if (kbd->method->KeyPressed(kbd, ' '))
	{
		this->actor->method->Move(this->actor, (VMFLOAT3A) {  0.0f,(float)fElapsedTime, 0.0f });
	}
	if (kbd->method->KeyPressed(kbd, VK_SHIFT))
	{
		this->actor->method->Move(this->actor, (VMFLOAT3A) {  0.0f, -(float)fElapsedTime, 0.0f });
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

	base.Window.method->SetPalette(&base.Window, pPalette);

	this->bStop = false;
	this->actor = new(VActor);
	this->mesh = new(CubeDr);
	//this->physics = new(Physics);

	this->pActiveCamera = this->actor->ACamera;
	this->model = new(Model, "Models\\Nano\\nanosuit.obj");

	this->pPl->projection = VMMatrixPerspectiveLH(1.0f, (float)base.Output->nFrameHeight / (float)base.Output->nFrameLength, 0.5f, 40.0f);
	this->pLight->_base.Bind(this->pLight, this->pPl);
	this->pPl->Debug = &base.Window;
	return true;
}
bool virtual(OnUserUpdate)(void* self, double fElapsedSeconds)
{
	account(self);

	if (this->bStop)
		return true;

	//this->physics->method->Tick(this->physics);


	this->pPl->camera = this->pActiveCamera->method->GetViewMatrix(this->pActiveCamera);
	base.Output->method->BeginFrame(base.Output, ' ', BG_Sky);

	//this->mesh->method->Draw(this->mesh, this->pPl);
	//this->actor->Mesh->_base.method->Draw(this->actor->Mesh, this->pPl);
  	this->model->method->Draw(this->model, this->pPl);
	//this->physics->pMesh->method->Draw(this->physics->pMesh, this->pPl);
	base.Output->method->PresentFrame(base.Output);
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

	override(OnUserCreate);
	override(HandleControls);
	override(OnUserUpdate);
	override(HandleInputEvents);
	override(OnUserDestroy);
	override(HandleMouse);

	return this;
}
Destructor(void* self)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->dtor(self);
	return this;
}
ENDCLASSDESC