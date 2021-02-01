#pragma once

#define T char
#define N 16
#include <RingT.h>
#undef T
#undef N
#define N 256
#include <BitSetT.h>
#undef N
#define T Bindable
#include <Shared_ptr.h>
#undef T