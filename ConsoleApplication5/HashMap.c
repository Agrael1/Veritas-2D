#include "HashMap.h"
#include "Class.h"
#include <malloc.h>
#include <string.h>

#define CHUNKSIZE 64
unsigned _Hash(const char *s)
{
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % CHUNKSIZE;
}

Node* virtual(find)(selfptr, char *key)
{
	Node *np;
	for (np = private.list[_Hash(key)]; np != nullptr; np = np->next)
		if (strcmp(key, np->key) == 0)
			return np; 
	return nullptr;
}
void* virtual(insert)(selfptr, char* key)
{
	Node *np;
	unsigned hashval;
	if (!(np = virtual(find)(self, key))) 
	{
		np = (Node*)malloc(sizeof(Node)-sizeof(void*)+self->elsize);
		if (!np || !(np->key = _strdup(key))) return nullptr;

		hashval = _Hash(key);
		np->next = private.list[hashval];

		private.list[hashval] = np;
		self->size++;
		return &np->info;
	}
	return nullptr;
}

VirtualTable{
	.find = virtual(find),
	.insert = virtual(insert)
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	private.list = calloc(CHUNKSIZE, sizeof(Node*));
	self->elsize = va_arg(*ap, unsigned);
	self->size = 0;
	return self;
}
Destructor(selfptr)
{
	free(private.list);
	return self;
}
ENDCLASSDESC