// Copyright (c) 2020
 //      Ilya Doroshenko, xdoros01
 //      agrael@live.ru, xdoros01@fit.vutbr.cz
 // All rights reserved.
 //
 // This code is licensed under the MIT License.
 //
 // Permission is hereby granted, free of charge, to any person obtaining a copy
 // of this software and associated documentation files(the "Software"), to deal
 // in the Software without restriction, including without limitation the rights
 // to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 // copies of the Software, and to permit persons to whom the Software is
 // furnished to do so, subject to the following conditions :
 //
 // The above copyright notice and this permission notice shall be included in
 // all copies or substantial portions of the Software.
 //
 // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 // THE SOFTWARE.


#if defined(T)&&defined(N)&&N<=255
#include <OptionalT.h>
#include <Class.h>
#include <VDefs.h>

#define Ring(x,n) concat4(c,Ring, x, concat3(_,n,_))
#define RingT(x,n) concat3(Ring, x, concat3(_,n,_))
#define c_class RingT(T,N)

struct __rconcat(c_class, _base)
{
	T container[N];
	unsigned char contains;
	unsigned char current;
};
interface
{
	Optional(T) (*pop)(selfptr);
	void (*push)(selfptr, T value);
	void (*wipe)(selfptr);
};
ComposeTypeEx;

inline void virtual(push)(selfptr, T value)
{
	if (self->current + 1 < N)
	{
		self->container[self->current++] = value;
	}
	else
	{
		self->container[self->current] = value;
		self->current = 0;
	}
	if (self->contains != N)
		self->contains++;
}
inline Optional(T) virtual(pop)(selfptr)
{
	register unsigned char index = 0;
	if (self->contains > 0)
	{
		if ((short)self->current - self->contains > 0)
		{
			index = self->current - self->contains;
		}
		else
		{
			index = self->current + N - self->contains;
		}
		self->contains--;
		return (Optional(T)) { self->container[index], true };
	}
	return nullopt(T);
}
inline void virtual(wipe)(selfptr)
{
	self->contains = 0;
	self->current = 0;
}

TMethodTable{
	.pop = virtual(pop),
	.push = virtual(push),
	.wipe = virtual(wipe)
};
inline Constructor(selfptr, va_list* ap)
{
	InitializeVtableEx(self);
	self->contains = 0;
	self->current = 0;
}
ENDTEMPLATEDESCDD
#undef c_class
#endif