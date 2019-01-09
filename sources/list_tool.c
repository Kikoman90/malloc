#include "malloc.h"

t_mem   g_memory = {0, 0, 0};

t_memchunck *memchunck_create(void)
{
    t_memchunck *memchunck;

    if ((memchunck = mmap(0, sizeof(t_memchunck), PROT_READ | PROT_WRITE, \
        MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
        return (NULL);

    memchunck->size = 0;
    memchunck->prev = NULL;
    memchunck->next = NULL;
    
    return (memchunck);
}

t_memchunck *link_chunck(t_memchunck *memchunck)
{
    int i; 
    t_memchunck *save;
    t_memchunck *prev;

    save = memchunck;
    i = 0;
    while (i < MAX_ALLOC)
    {
        prev = memchunck;
        memchunck->next = memchunck_create();
        memchunck->next->next = NULL;
        memchunck = memchunck->next;
        memchunck->prev = prev;
        i++;
    }
    return (save);
}

t_memzone *memzone_create(size_t size)
{
    t_memzone *memzone;
    
    if ((memzone = mmap(0, sizeof(t_memzone), PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
    /*if ((memzone->addr = mmap(0, size * MAX_ALLOC, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);*/
    memzone->alloc = NULL;
    memzone->free = NULL;
    //memzone->pool = link_chunck(NULL);
    memzone->prev = NULL;
    memzone->next = NULL;

    return (memzone);
}