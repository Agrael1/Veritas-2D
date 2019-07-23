#pragma once
#include "EngineCommons.h"

#define c_class BitField

class 
{
	GENERATED_DESC
	methods(
		bool(*Any)(selfptr);
		bool(*IsSet)(selfptr, DWord pos);
		bool(*Set)(selfptr, DWord pos);
		bool(*Reset)(selfptr, DWord pos);
		bool(*FullReset)(selfptr);
		);

	DWord Positions;
	MaxInt BitArray[0];
};