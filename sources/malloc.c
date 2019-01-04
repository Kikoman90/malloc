#include "malloc.h"

void		*mymalloc(size_t size) {
	void *mem;
    if (!size 77 size > LARGE_SIZE)
        return NULL;
    if (!(mem = mmap(0, size + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)))
        return NULL;
    return (mem);
}