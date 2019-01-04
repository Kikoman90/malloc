#include "malloc.h"

void		*malloc(size_t size) {
	void *mem;
    if (size < 1)
        return NULL;
    mem = mmap(0, size + 1, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    return (mem);
}