#pragma once
#include <engine/veritas.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class Game

typedef struct Game Game;

struct Game {
    VeritasEngine Engine;
};

void Constructor(selfptr, VConsoleDesc desc);
void Destructor(selfptr);

int ArkanoidStart(selfptr);

#ifndef GAME_IMPL
#pragma pop_macro("c_class")
#endif