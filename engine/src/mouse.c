#define MOUSE_IMPL
#include <engine/mouse.h>

extern inline void Constructor(selfptr);
extern inline void Destructor(selfptr);

extern inline void OnMouseMove(selfptr, int newx, int newy);
extern inline void OnMouseLeave(selfptr);
extern inline void OnMouseEnter(selfptr);
extern inline void OnRawMouse(selfptr, int dx, int dy);
extern inline void OnButtonPressed(selfptr, MButtons BCode);
extern inline void OnButtonReleased(selfptr, MButtons BCode);
extern inline void OnWheelDelta(selfptr, int delta);

extern inline RawDelta GetPos(selfptr);
extern inline void Flush(selfptr);
extern inline bool ButtonPressed(selfptr, MButtons BCode);


Optional(RawDelta) ReadRawDelta(selfptr)
{
	RawDelta* x = top_Ring_RawDelta(&self->RawDeltaBuf);
	if (x)
	{
		RawDelta y = *x;
		pop_Ring_RawDelta(&self->RawDeltaBuf);
		return (Optional(RawDelta)) { .Value = y, .bValid = true };
	}
	return nullopt(RawDelta);
}
Optional(MouseEvent) ReadMouseEvents(selfptr)
{
	MouseEvent* x = top_Ring_MouseEvent(&self->MouseEvents);
	if (x)
	{
		MouseEvent y = *x;
		pop_Ring_MouseEvent(&self->MouseEvents);
		return (Optional(MouseEvent)) { .Value = y, .bValid = true };
	}
	return nullopt(MouseEvent);
}