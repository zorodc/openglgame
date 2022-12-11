#ifndef UTIL_H
#define UTIL_H
#include "common.h"
#include <string.h>

#define MK_CHARBUF(N)                                   \
    ((charbuf)memcpy(ALLOCA(sizeof(struct charbuf)+(N)),\
                   &(size_t){(N)}, sizeof(size_t)))
struct charbuf {
	size_t cap;
	char   buf[];
};
typedef struct charbuf* charbuf;

static inline
const void* lookup(size_t esz, const void *const key, size_t cnt, const void *arr)
{
	for (const char* itr = (char*)arr; cnt; --cnt)
		if (0 == memcmp(itr, key, esz)) return itr;
		else                            itr += esz;
	return NULL;
}

#endif//UTIL_H
