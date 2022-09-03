#define HASH_MAP_IMPL
#include "HashMap.h"
#include "Class.h"
#include <assert.h>
#include <malloc.h>
#include <va_raw.h>

#define CHUNKSIZE 64


struct HashMap_base_
{
	union T_Info
	{
		const struct Class* _class;
		size_t elsize;
	};
	virtual(Node)* es[CHUNKSIZE];
};

size_t _Hash(const char *s)
{
	size_t hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % CHUNKSIZE;
}

virtual(Node)* virtual(find)(selfptr, String key)
{
	for (virtual(Node)*np = self->pimpl->es[_Hash(c_str(&key))]; np != NULL; np = np->next)
		if (string_cmp(&key, &np->key))
			return np;
	return NULL;
}
virtual(Node)* virtual(find_c)(selfptr, const char* key)
{
	for (virtual(Node)*np = self->pimpl->es[_Hash(key)]; np != NULL; np = np->next)
		if (strcmp(key, c_str(&np->key)) == 0)
			return np;
	return NULL;
}

void* virtual(try_emplace)(selfptr, String key, ...)
{
	va_list ap;
	va_start(ap, key);

	virtual(Node)* np;
	size_t hashval;

	if (!(np = virtual(find)(self, key))) 
	{
		np = calloc(1, sizeof(virtual(Node)) + self->pimpl->_class->size - sizeof(void*));
		assert(np);
		string_copy(&np->key, &key);
		hashval = _Hash(c_str(&key));
		np->next = self->pimpl->es[hashval];

		if (self->pimpl->_class->ctor)
			self->pimpl->_class->ctor(np->element, &ap);

		self->pimpl->es[hashval] = np;
		self->elements++;
		return np->element;
	}
	return NULL;
}
void* virtual(try_emplace_intr)(selfptr, String key, ...)
{
	va_list ap;
	va_start(ap, key);

	virtual(Node)* np;
	size_t hashval;

	if (!(np = virtual(find)(self, key)))
	{
		np = calloc(1, sizeof(virtual(Node)) + self->pimpl->elsize);
		assert(np);
		string_copy(&np->key, &key);
		hashval = _Hash(c_str(&key));
		np->next = self->pimpl->es[hashval];

		va_raw(np->element, ap, self->pimpl->elsize);

		self->pimpl->es[hashval] = np;
		self->elements++;
	}
	return NULL;
}
void virtual(insert)(selfptr, String key, void** value)
{
	virtual(Node)* np;
	size_t hashval;
	if (!(np = virtual(find)(self, key)))
	{
		const struct Class* pclass = *(const struct Class**) value;
		if (pclass != self->pimpl->_class) return;

		np = calloc(1, sizeof(virtual(Node)) + pclass->size - sizeof(void*));
		assert(np);

		string_copy(&np->key, &key);
		hashval = _Hash(c_str(&key));
		np->next = self->pimpl->es[hashval];

		memcpy(np->element, (char*)value + sizeof(void*), pclass->size - sizeof(void*));

		self->pimpl->es[hashval] = np;
		self->elements++;
	}
}

void virtual(remove)(selfptr, String key)
{
	virtual(Node)*npprev = NULL;
	const struct Class* pclass = self->pimpl->_class;
	for (virtual(Node)*np = self->pimpl->es[_Hash(c_str(&key))]; np != NULL; npprev = np, np = np->next)
		if (string_cmp(&key, &np->key))
		{
			if (pclass->dtor)
			{
				pclass->dtor(np->element);
			}
			string_remove(&np->key);
			free(np);
			npprev->next = NULL;
			self->elements--;
		}
}
void virtual(remove_intr)(selfptr, String key)
{
	virtual(Node)*npprev = NULL;
	for (virtual(Node)*np = self->pimpl->es[_Hash(c_str(&key))]; np != NULL; npprev = np, np = np->next)
		if (string_cmp(&key, &np->key))
		{
			string_remove(&np->key);
			free(np);
			npprev->next = NULL;
			self->elements--;
		}
}

static void clear(virtual(Node)*np, void(*dtor)(void*))
{
	if (np->next)
	{
		clear(np, dtor);
	}
	else
	{
		dtor(np->element);
		string_remove(&np->key);
		free(np);
	}
}
static void clear_intr(virtual(Node)*np)
{
	if (np->next)
	{
		clear_intr(np);
	}
	else
	{
		string_remove(&np->key);
		free(np);
	}
}

VirtualTable{
	.find = virtual(find),
	.find_c = virtual(find_c),
	.try_emplace = virtual(try_emplace),
	.insert = virtual(insert),
	.remove = virtual(remove)
};
interface HashMap2 = {
	.find = virtual(find),
	.find_c = virtual(find_c),
	.try_emplace = virtual(try_emplace_intr),
	.remove = virtual(remove_intr),
	.insert = virtual(try_emplace_intr)
};

Constructor(selfptr, va_list *ap)
{
	const struct Class* cltbl = va_arg(*ap, void*);
	self->pimpl = calloc(1, sizeof(struct HashMap_base_));
	self->elements = 0;
	assert(self->pimpl);
	if (cltbl)
	{
		self->pimpl->_class = cltbl;
		InitializeVtable(self);
	}
	else
	{
		self->pimpl->elsize = va_arg(*ap, size_t);
		self->method = &HashMap2;
	}
}
Destructor(selfptr)
{
	virtual(Node)*np = NULL;
	if (self->method != &HashMap2)
	{
		void(*dtor)(void*) = ((struct Class*)self->pimpl->_class)->dtor;
		for (size_t elpt = 0; elpt < CHUNKSIZE; elpt++)
		{
			np = self->pimpl->es[elpt];
			if (np)
			{
				clear(np, dtor);
			}
		}
	}
	else
		for (size_t elpt = 0; elpt < CHUNKSIZE; elpt++)
		{
			np = self->pimpl->es[elpt];
			if (np)
			{
				clear_intr(np);
			}
		}

	free(self->pimpl);
}
ENDCLASSDESC