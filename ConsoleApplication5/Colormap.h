#pragma once

typedef struct _ColorMap
{
	float Width;
	float Height;
	
	unsigned long Stride;
	struct _CHAR_INFO* map;
}ColorMap;