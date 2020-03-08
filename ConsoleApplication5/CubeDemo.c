#include "VActor.h"
#include "CubeTexDr.h"
#include "Drawable.h"
#include "Model.h"
#include "Codex.h"
#include "TextureBuffer.h"

#include "Class.h"
#include "CubeDemo.h"
#include "L16TC.palette"

bool virtual(HandleInputEvents)(selfptr, const KeyboardEvent event)
{
	if (event.type == Press)
	{
		// check if the event was for the space key
		switch (event.code)
		{
		case VK_ESCAPE:
			return false;
		case 'R':
			self->bStop ^= true;
			break;
		case VK_INSERT:
			if (self->Control.bCursorEnabled)
			{
				self->Control.method->DisableCursor(&self->Control);
				self->Control.mouse.rawEnabled = true;
			}
			else
			{
				self->Control.method->EnableCursor(&self->Control);
				self->Control.mouse.rawEnabled = false;
			}
			break;
		}
	}
	return true;
}
void virtual(HandleControls)(selfptr, const struct Keyboard* kbd, double fElapsedTime)
{
	if (kbd->method->KeyPressed(kbd, 'W'))
	{
		self->actor->method->Move(self->actor, (VMFLOAT3A) { 0.0f, 0.0f, (float)fElapsedTime });
	}
	if (kbd->method->KeyPressed(kbd, 'S'))
	{
		self->actor->method->Move(self->actor, (VMFLOAT3A) { 0.0f, 0.0f, -(float)fElapsedTime });
	}
	if (kbd->method->KeyPressed(kbd, 'A'))
	{
		self->actor->method->Move(self->actor, (VMFLOAT3A) { -(float)fElapsedTime, 0.0f, 0.0f, });
	}
	if (kbd->method->KeyPressed(kbd, 'D'))
	{
		self->actor->method->Move(self->actor, (VMFLOAT3A) { (float)fElapsedTime, 0.0f, 0.0f });
	}
	if (kbd->method->KeyPressed(kbd, ' '))
	{
		self->actor->method->Move(self->actor, (VMFLOAT3A) {  0.0f,(float)fElapsedTime, 0.0f });
	}
	if (kbd->method->KeyPressed(kbd, VK_SHIFT))
	{
		self->actor->method->Move(self->actor, (VMFLOAT3A) {  0.0f, -(float)fElapsedTime, 0.0f });
	}
}
void virtual(HandleMouse)(selfptr, struct Mouse* mouse, double fElapsedTime)
{
	if (!self->Control.bCursorEnabled)
	{
		short X; short Y;
		mouse->method->ReadMouseMovement(mouse, &X, &Y);
		float dX = (float)X*(float)fElapsedTime;
		float dY = (float)Y*(float)fElapsedTime;
		self->pActiveCamera->method->Rotate(self->pActiveCamera, dX, dY);
		if (mouse->WheelDelta != 0)
			self->pActiveCamera->method->Translate(self->pActiveCamera,
			(VMFLOAT3A) {
			0.0f, 0.0f, (float)fElapsedTime*(float)mouse->method->ReadWheelDelta(mouse)
		});
	}
}

bool virtual(OnUserCreate)(selfptr)
{
	InitializeCodex();
	self->pPl = new(VLine, self->Output);
	self->pLight = new(DirectionalLight);

	self->Window.method->SetPalette(&self->Window, pPalette);

	self->bStop = false;
	self->actor = new(VActor);
	self->mesh = new(CubeDr);
	//self->physics = new(Physics);

	self->pActiveCamera = self->actor->ACamera;
	self->model = new(Model, "Models\\Nano\\nanosuit.obj");

	self->pPl->projection = VMMatrixPerspectiveLH(1.0f, (float)self->Output->nFrameHeight / (float)self->Output->nFrameLength, 0.5f, 40.0f);
	self->pLight->Bind(self->pLight, self->pPl);
	self->pPl->Debug = &self->Window;
	return true;
}
bool virtual(OnUserUpdate)(selfptr, double fElapsedSeconds)
{
	if (self->bStop)
		return true;

	//self->physics->method->Tick(self->physics);


	self->pPl->camera = self->pActiveCamera->method->GetViewMatrix(self->pActiveCamera);
	self->Output->method->BeginFrame(self->Output, (CHAR_INFO){ ' ', BG_Sky });

	//self->mesh->method->Draw(self->mesh, self->pPl);
	//self->actor->Mesh->_base.method->Draw(self->actor->Mesh, self->pPl);
  	self->model->method->Draw(self->model, self->pPl);
	//self->physics->pMesh->method->Draw(self->physics->pMesh, self->pPl);
	self->Output->method->PresentFrame(self->Output);
	return true;
}
bool virtual(OnUserDestroy)(selfptr)
{
	delete_s(self->mesh);
	delete_s(self->actor);
	delete_s(self->pPl);
	delete_s(self->model);
	delete_s(self->pLight);

	DestroyCodex();
	return true;
}

Constructor(void* self, va_list *ap)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->ctor(self, ap);
	this->AppName = typeOf_t(CubeDemo);

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