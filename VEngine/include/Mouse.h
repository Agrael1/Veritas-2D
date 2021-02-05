#pragma once
#include <RuntimeClass.h>
#include <stdbool.h>
#include <limits.h>

typedef enum
{
	LEFT_MB,
	RIGHT_MB,
	MID_MB
}MButtons;
typedef struct RawDelta
{
	int x, y;
}RawDelta;
typedef enum
{
	MInvalid,
	LPress,
	LRelease,
	RPress,
	RRelease,
	MPress,
	MRelease,
	WheelUp,
	WheelDown,
	Move,
	Enter,
	Leave,
}MouseEventType;

/// @brief Struct that has all the information about mouse events
typedef struct MouseEvent
{
	MouseEventType type;
	struct
	{
		bool bLeftIsPressed : 1;
		bool bRightIsPressed : 1;
		bool bmiddleIsPressed : 1;
		bool padding : sizeof(bool)* CHAR_BIT - 3;
	};
	int x;			///<x position where the event occured
	int y;			///<y position where the event occured
}MouseEvent;

inline void MakeMouseEvent(MouseEvent* self, MouseEventType type, struct Mouse* in);

#define RING_T MouseEvent
#include <VRingT.h>

#define RING_T RawDelta
#include <VRingT.h>

#define OPTIONAL_T MouseEvent
#include <VOptionalT.h>

#define OPTIONAL_T RawDelta
#include <VOptionalT.h>

#define WHEEL_DELTA 120

#pragma push_macro("c_class")
#undef c_class
#define c_class Mouse

typedef struct Mouse Mouse;

struct Mouse
{
	Ring(MouseEvent) MouseEvents;	///<Buffer for mouse events
	Ring(RawDelta) RawDeltaBuf;		///<Buffer for raw delta

	short posX, posY;
	int WheelDelta;
	bool bIsInWindow;
	bool bLeftIsPressed;
	bool bRightIsPressed;
	bool bMidIsPressed;
	bool bRawEnabled;
};

inline void Constructor(selfptr)
{
	Ring_MouseEvent_ctor(&self->MouseEvents, 16);
	Ring_RawDelta_ctor(&self->RawDeltaBuf, 16);

	self->posX = self->posY = self->
		WheelDelta = self->
		bIsInWindow = self->
		bLeftIsPressed = self->
		bRightIsPressed = self->
		bMidIsPressed = self->
		bRawEnabled = 0;
}
inline void Destructor(selfptr)
{
	Ring_MouseEvent_dtor(&self->MouseEvents);
	Ring_RawDelta_dtor(&self->RawDeltaBuf);
}

/// @brief Writes move event to event queue
/// @param newx - x of new position in pixels
/// @param newy - y of new position in pixels
inline void OnMouseMove(selfptr, int newx, int newy)
{
	self->posX = newx;
	self->posY = newy;

	MakeMouseEvent(emplace_Ring_MouseEvent(&self->MouseEvents), Move, self);
}

/// @brief Writes leave event to event queue 
inline void OnMouseLeave(selfptr)
{
	self->bIsInWindow = false;
	MakeMouseEvent(emplace_Ring_MouseEvent(&self->MouseEvents), Leave, self);
}

/// @brief Writes leave event to event queue 
inline void OnMouseEnter(selfptr)
{
	self->bIsInWindow = true;
	MakeMouseEvent(emplace_Ring_MouseEvent(&self->MouseEvents), Enter, self);
}

/// @brief If Raw mouse movement is enabled then writes raw delta to the buffer
/// @param dx - delta width in pixels
/// @param dy - delta height in pixels
inline void OnRawMouse(selfptr, int dx, int dy)
{
	*emplace_Ring_RawDelta(&self->RawDeltaBuf) = (RawDelta){ dx,dy };
}

/// @brief Sets state of the respective button to pressed and adds event to the queue
/// @param BCode - which button was pressed
inline void OnButtonPressed(selfptr, MButtons BCode)
{
	MouseEventType e = MInvalid;
	switch (BCode)
	{
	case LEFT_MB:
		e = LPress;
		self->bLeftIsPressed = true;
	case RIGHT_MB:
		e = RPress;
		self->bRightIsPressed = true;
	case MID_MB:
		e = MPress;
		self->bMidIsPressed = true;
	default:
		break;
	}

	MakeMouseEvent(emplace_Ring_MouseEvent(&self->MouseEvents), e, self);
}

/// @brief Sets state of the respective button to released and adds event to the queue
/// @param BCode - which button was released 
inline void OnButtonReleased(selfptr, MButtons BCode)
{
	MouseEventType e = MInvalid;
	switch (BCode)
	{
	case LEFT_MB:
		e = LRelease;
		self->bLeftIsPressed = false;
	case RIGHT_MB:
		e = RRelease;
		self->bRightIsPressed = false;
	case MID_MB:
		e = MRelease;
		self->bMidIsPressed = false;
	default:
		break;
	}

	MakeMouseEvent(emplace_Ring_MouseEvent(&self->MouseEvents), e, self);
}

/// @brief Wraps all wheel deltas to the events and adds them to the queue
/// Delta is 120
/// @param Delta - How much wheel was rotated, must be divisible by 120!
inline void OnWheelDelta(selfptr, int delta)
{
	self->WheelDelta += delta;

	while (self->WheelDelta >= WHEEL_DELTA)
	{
		self->WheelDelta -= WHEEL_DELTA;
		MakeMouseEvent(emplace_Ring_MouseEvent(&self->MouseEvents), WheelUp, self);
	}
	while (self->WheelDelta <= -WHEEL_DELTA)
	{
		self->WheelDelta += WHEEL_DELTA;
		MakeMouseEvent(emplace_Ring_MouseEvent(&self->MouseEvents), WheelDown, self);
	}
}

/// @brief Retrieves current position of the cursor 
/// @return position of the cursor in pixels
inline RawDelta GetPos(selfptr)
{
	return (RawDelta) { self->posX, self->posY };
}

/// @brief Resets all the buffers 
inline void Flush(selfptr)
{
	clear_Ring_MouseEvent(&self->MouseEvents);
	clear_Ring_RawDelta(&self->RawDeltaBuf);
}

/// @brief Checks wether some button is pressed
/// @param BCode Which button to check
/// @return button current state
inline bool ButtonPressed(selfptr, MButtons BCode)
{
	switch (BCode)
	{
	case LEFT_MB: return self->bLeftIsPressed;
	case RIGHT_MB: return self->bRightIsPressed;
	case MID_MB: return self->bMidIsPressed;
	default: return false;
	}
}

/// @brief Gets one RawDelta from the queue
/// @return RawDelta event if queue is not empty, nullopt otherwise
Optional(RawDelta) ReadRawDelta(selfptr);

/// @brief Gets one event from the queue
/// @return MouseEvent if queue is not empty, nullopt otherwise
Optional(MouseEvent) ReadMouseEvents(selfptr);

/// @brief Automatic constructor of the mouse event from mouse state
/// @param self - pointer to uninitialized mouse event
/// @param type - which type of event to construct
/// @param mouse - mouse state to get info from
inline void MakeMouseEvent(MouseEvent* self, MouseEventType type, Mouse* mouse)
{
	self->bLeftIsPressed = mouse->bLeftIsPressed;
	self->bmiddleIsPressed = mouse->bMidIsPressed;
	self->bRightIsPressed = mouse->bRightIsPressed;

	self->type = type;
	self->x = mouse->posX;
	self->y = mouse->posY;
}

#ifndef MOUSE_IMPL
#pragma pop_macro("c_class")
#endif
