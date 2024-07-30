#define VENGINE_IMPL
#include <engine/veritas.h>

volatile BOOL bActive = false;

extern inline void Present(selfptr);

bool Virtual(HandleInputEvents)(void* self, const KeyboardEvent event)
{
    if (event.type == Press) {
        // check if the event was for the escape key
        if (event.code == VK_ESCAPE)
            return false;
    }
    return true;
}
bool PassEvents(selfptr)
{
    Optional(KeyboardEvent) event = { 0 };

    while ((event = ReadKey(&self->Control.kbd)).bValid) {
        return self->vptr->HandleInputEvents(self, event.Value);
    }
    return true;
}

void _CreateDevice(selfptr)
{
    // self->pDevice = new(VLine, self->pSwap);
}
void _CreateDeviceAndSwapChain(selfptr, VConsoleDesc VPDesc)
{
    _CreateDevice(self);
}
DWORD GameThread(selfptr)
{
    int gResult = 0;
    Optional(WPARAM) message = { 0 };

    // Time counting
    LARGE_INTEGER StartingTime, EndingTime;
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);

    if (!self->vptr->OnUserCreate(self))
        return 2;

    // first try
    QueryPerformanceCounter(&StartingTime);
    if (!self->vptr->OnUserUpdate(self, self->fElapsedSeconds)) {
        gResult = 4;
        bActive = false;
    } else {
        bActive = true;
    }

    // Game Loop
    while (bActive) {
        QueryPerformanceCounter(&EndingTime);
        // Catch a focus if not in it
        if (!self->Control.bInFocus) {
            if (CatchFocus(&self->Window))
                ControlAwake(&self->Control);
        }

        // Read Messages
        if ((message = ProcessMessages()).bValid) {
            bActive = false;
            break;
        };

        // Process queue
        if (!PassEvents(self)) {
            gResult = 1;
            bActive = false;
            break;
        };

        // Process mouse
        if (self->vptr->HandleMouse)
            self->vptr->HandleMouse(self, &self->Control.mouse, self->fElapsedSeconds);

        // Process continuous input
        if (self->vptr->HandleControls)
            self->vptr->HandleControls(self, &self->Control.kbd, self->fElapsedSeconds);

        // render frame
        if (!self->vptr->OnUserUpdate(self, self->fElapsedSeconds)) {
            gResult = 0;
            bActive = false;
            break;
        };

        self->fElapsedSeconds = (float)(EndingTime.QuadPart - StartingTime.QuadPart) / (float)Frequency.QuadPart;
        StartingTime = EndingTime;

        // present frame
        Present(self);
    }

    if (self->vptr->OnUserDestroy && !self->vptr->OnUserDestroy(self)) {
        gResult = 3;
    }
    return gResult;
}
int Start(selfptr)
{
    // parse game output (or not)
    return GameThread(self);
}

VirtualTable(IVeritasEngine){
    .HandleInputEvents = Virtual(HandleInputEvents),
};

void Constructor(selfptr, VConsoleDesc screenparams)
{
    self->vptr = &vfptr;
    self->fElapsedSeconds = 0.0f;
    ConsoleWindow_ctor(&self->Window);
    COORD frame = CreateConsole(&self->Window, screenparams.width, screenparams.height, screenparams.fontw, screenparams.fonth);
    SwapChain_ctor(&self->Swap, frame);
    MessageWindow_ctor(&self->Control, frame.X, frame.Y, self->Window.ConsoleWindow);
}
void Destructor(selfptr)
{
    SwapChain_dtor(&self->Swap);
    MessageWindow_dtor(&self->Control);
    ConsoleWindow_dtor(&self->Window);
}