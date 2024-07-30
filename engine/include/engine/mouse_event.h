#pragma once
#include <limits.h>
#include <stdbool.h>

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
	MMove,
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
#include <vlib/ring_t.h>

#define RING_T RawDelta
#include <vlib/ring_t.h>

#define OPTIONAL_T MouseEvent
#include <vlib/optional_t.h>

#define OPTIONAL_T RawDelta
#include <vlib/optional_t.h>