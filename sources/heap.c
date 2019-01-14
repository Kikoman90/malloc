// 42 header

#include "malloc.h"

t_metapool  *create_metapool(size_t nb_meta)
{
	t_metapool	*pool;
	t_meta		*tmp;
	void		*end;

	if ((pool = mmap(0, sizeof(t_meta) * nb_meta, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (log_error_null("//", strerror(errno))); // incomplete
	end = (void*)(pool + (nb_meta - 1));
	pool->pool = (t_meta*)(pool + 1);
	tmp = pool->pool;
	while ((void*)tmp < end)
	{
		tmp->next = tmp + 1;
		tmp->next->prev = tmp;
		tmp = tmp->next; 
	}
	tmp->next = NULL;
	pool->size = sizeof(t_meta) * nb_meta;
	pool->prev = NULL;
	return (pool);
}

t_memzone   *create_memzone(size_t chunck_size)
{
	t_memzone	*zone;

	if ((zone = mmap(0, chunck_size * MAX_ALLOC, PROT_READ | PROT_WRITE, \
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (log_error_null("//", strerror(errno))); // incomplete
	if (!(zone->pool = create_metapool(4096)))
		return (log_error_null("//", "//")); // incomplete
    zone->pool->next = NULL;
	if (!(zone->meta = metadip(zone->pool, (void*)(zone + 1), \
		(chunck_size * MAX_ALLOC) - sizeof(t_memzone))))
        return (log_error_null("//", "//")); // incomplete
	zone->meta->next = NULL;
	zone->prev = NULL;
	return (zone);
}

int     	destroy_metapools(t_metapool *pool)
{
    t_metapool  *tmp;

    while (pool)
    {
        tmp = pool;
        pool = pool->next;
        if (munmap(tmp, tmp->size) == -1)
            return (log_error("//", strerror(errno))); //incomplete
    }
    return (TRUE);
}

int     	destroy_memzone(t_memzone *zone, size_t size)
{
    zone->meta = NULL;
    if (zone->prev)
        zone->prev->next = zone->next;
    if (zone->next)
        zone->next->prev = zone->prev;
    if (!destroy_metapools(zone->pool))
        return (FALSE); // check
    zone->pool = NULL;
    if (munmap(zone, size) == -1)
        return (log_error("//", strerror(errno))); //incomplete
    return (TRUE);
}