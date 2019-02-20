#ifndef New_h
#define New_h

void* new (const void* type, ...);
void delete(void* item);

void* clone(const void* self);								// copy func to copy properly
int	differ(const void* self, const void* b);				// just a simple ass comparator
unsigned int sizeOf(const void* self);

#endif // !New

