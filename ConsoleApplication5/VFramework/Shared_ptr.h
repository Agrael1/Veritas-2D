#if defined(T)
#include <VDefs.h>

#define shared_ptr(x) concat3(shared_ptr,_,x)
#define weak_ptr(x) weak_ptr

typedef struct 
{
	const void* _class;
	void* _ctrl_ptr;
}weak_ptr;

typedef struct
{
	struct _control_block* control;
	T* ptr;
}shared_ptr(T);


void copy_shared(void** self, void** to);
void make_shared(void** _where, const void* type, ...);
void attach_shared(void** _where, void** from);
void weak_from(void** pShared, void** destWeak); //@self - shared pointer to construct from
void* _weak_lock_unsafe(weak_ptr pWeak); //check for NULL;
#define weak_lock(pWeak, ...) ((__VA_ARGS__*)_weak_lock_unsafe(pWeak))
#endif