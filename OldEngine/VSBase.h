#pragma once
#include "VeritasMath.h"
#include "IndexedTriangleList.h"
#include "VDefs.h"

struct VSBase
{
	void* ConstantBuffers[2];
	unsigned VSOutSize;
};
struct VSBase_vtbl
{
	void(*Apply)(void* self, void** _out, void** _in, void* buffers[2]);
};

inline VMVECTOR* VSOutInterpolate(void* _out, SVMVECTOR* v0, SVMVECTOR* v1, float alpha, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorLerp(v0[i].v, v1[i].v, alpha);
	return _out;
}
inline VMVECTOR* VSOutScale(void* _out, SVMVECTOR* v0, float alpha, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorScale(v0[i].v, alpha);
	return _out;
}
inline VMVECTOR* VSOutSubtract(void* _out, SVMVECTOR* v0, SVMVECTOR* v1, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorSubtract(v0[i].v, v1[i].v);
	return _out;
}
inline VMVECTOR* VSOutAdd(void* _out, SVMVECTOR* v0, SVMVECTOR* v1, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorAdd(v0[i].v, v1[i].v);
	return _out;
}
