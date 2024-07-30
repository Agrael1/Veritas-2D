#pragma once
#include <stdint.h>

enum Type
{
	Invalid = 0,
	Press,
	Release,
};
typedef struct
{
	uint8_t type;
	uint8_t code;
}KeyboardEvent;

#define RING_T KeyboardEvent
#include <vlib/ring_t.h>

#define RING_T char
#include <vlib/ring_t.h>

#define OPTIONAL_T KeyboardEvent
#include <vlib/optional_t.h>

#define OPTIONAL_T char
#include <vlib/optional_t.h>