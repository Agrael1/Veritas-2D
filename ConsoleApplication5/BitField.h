#pragma once
#include "EngineCommons.h"

#define c_class BitField

class 
{
	GENERATED_DESC
	DWord Positions;

	methods(
		bool(*Any)(selfptr);
		bool(*IsSet)(selfptr, DWord pos);
		bool(*Set)(selfptr, DWord pos);
		bool(*Reset)(selfptr, DWord pos);
		bool(*FullReset)(selfptr);
		);

	MaxInt BitArray[0];
};