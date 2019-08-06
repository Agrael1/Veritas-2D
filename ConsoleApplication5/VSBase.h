#pragma once
#include "VeritasMath.h"
#include "ShaderCommons.h"

#define c_class VSBase

class
{
	GENERATED_DESC
	void(*Apply)(selfptr, void* _out, struct IndexedTriangleList* _in);
	unsigned VSOutSize;
};

inline VMVECTOR* VSOutInterpolate(void* _out, SVMVECTOR* v0, SVMVECTOR* v1, float alpha, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorLerp(v0->v, v1->v, alpha);
	return _out;
}
inline VMVECTOR* VSOutScale(void* _out, SVMVECTOR* v0, float alpha, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorScale(v0->v, alpha);
	return _out;
}
inline VMVECTOR* VSOutSubtract(void* _out, SVMVECTOR* v0, SVMVECTOR* v1, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorSubtract(v0->v, v1->v);
	return _out;
}
inline VMVECTOR* VSOutAdd(void* _out, SVMVECTOR* v0, SVMVECTOR* v1, size_t VSize)
{
	for (unsigned i = 0; i < VSize / 16; i++)
		((VMVECTOR*)_out)[i] = VMVectorAdd(v0->v, v1->v);
	return _out;
}
