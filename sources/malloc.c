// header 42

#include "malloc.h"

t_mem			g_memory = {0, 0, 0};
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

static t_metapool	*create_metapool(size_t nb_elem)
{
	t_metapool	*pool;
	t_meta		*tmp;
	void		*end;

	if ((pool = mmap(0, sizeof(t_meta) * nb_elem, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		// if (DEBUG_MALLOC) ...
		return (NULL);
	}
	end = (void*)(pool + (nb_elem - 1));
	pool->pool = (t_meta*)(pool + 1);
	tmp = pool->pool;
	while ((void*)tmp < end)
	{
		tmp->next = tmp + 1;
		tmp->next->prev = tmp;
		tmp = tmp->next; 
	}
	tmp->next = NULL;
	pool->size = sizeof(t_meta) * nb_elem;
	pool->prev = NULL;
	return (pool);
}

/*
** put a t_meta back in the pool
*/
void			metathrow(t_metapool *pool, t_meta *elem)
{
	elem->prev = NULL;
	elem->next = pool->pool;
	pool->pool->prev = elem;
	pool->pool = elem;
}

/*
** grab a t_meta from the pool 
** NECESARRY TO MUNMAP POOLS AT A CERTAIN POINT
** (#define POOL_SIZE 4096) ?
*/

// t_metapool *pool or t_metapool **pool
t_meta			*metadip(t_metapool *metapool, void *addr, size_t size)
{
	t_metapool	*pool;
	t_meta		*elem;

	if (!metapool || !metapool->pool)
	{
		if (!(pool = create_metapool(4096)))
			return (NULL);
		pool->next = metapool;
		metapool = pool;
	}
	pool = metapool;
	elem = pool->pool;
	elem->next->prev = NULL;
	pool->pool = elem->next;
	elem->addr = addr;
	elem->size = size;
	return (elem);
}

t_memzone		*create_memzone(size_t chunck_size)
{
	t_memzone	*zone;
	t_meta		*free_elem;

	if ((zone = mmap(0, chunck_size * MAX_ALLOC, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		// if (MALLOC_DEBUG) ...
		return (NULL);
	}
	if (!(zone->pool = create_metapool(4096)))
	{
		// if (!munmap(zone))
		// ... error
		return (NULL);
	}
	if (!(zone->meta = metadip(zone->pool, (void*)(zone + 1), (chunck_size * MAX_ALLOC) - sizeof(t_memzone))))
	{
		// yeah yeah...
		return (NULL);
	}
	zone->meta->next = NULL;
	zone->prev = NULL;
	return (zone);
}

t_meta			*insert_free(t_metapool *pool, t_meta *meta, void *addr, size_t size) {
	t_meta	*insert;

	if (!(insert = metadip(pool, addr, size)))
		return (NULL);
	if (meta)
	{
		if (meta->next)
			meta->next->prev = insert;
		insert->next = meta->next;
		insert->prev = meta;
		meta->next = insert;
	}
	insert->used = 0;
	return (insert);
}

t_meta			*insert_alloc(t_metapool *pool, t_meta *meta, void *addr, size_t size) {
	t_meta	*insert;

	if (!(insert = metadip(pool, addr, size)))
		return (NULL);
	if (meta)
	{
		if (meta->next)
			meta->next->prev = insert;
		insert->next = meta->next;
		insert->prev = meta;
		meta->next = insert;
	}
	insert->used = 1;
	return (insert);
}

void			*new_alloc(t_meta *elem, t_memzone *zone, size_t size)
{
	if (elem->size > size && !insert_free(zone->pool, elem, \
		(void*)((char*)elem->addr + size), elem->size - size))
		return (NULL);
	elem->size = size;
	elem->used = 1;
	return (elem->addr);
}

void			*malloc_tiny_or_small(size_t size, size_t chunck_size, t_memzone **m_zone)
{
	t_memzone	*zone;
	t_meta		*meta;

	zone = *m_zone;
	while (zone)
	{
		meta = zone->meta;
		while (meta)
		{
			if (!meta->used && meta->size >= size)
				return (new_alloc(meta, zone, size));
			meta = meta->next;
		}
		zone = zone->next;
	}
	if (!(zone = create_memzone(chunck_size)))
		return (NULL);
	zone->next = *m_zone;
	*m_zone = zone;
	return (new_alloc(zone->meta, zone, size));
}

void			*malloc_large(size_t size)
{
	(void)size;
	return (NULL);
}

/*
** il faut une fonction qui s'assure que la taille des zones est un multiple de pagesize()
*/
void			*mymalloc(size_t size)
{
	if (!size || size > LARGE_CHUNCK_SIZE)
	{
		// if (MALLOC_DEBUG) ...
		return (NULL);
	}
	if (size <= TINY_CHUNCK_SIZE)
		return (malloc_tiny_or_small(size, TINY_CHUNCK_SIZE, &g_memory.tiny));
	else if (size <= SMALL_CHUNCK_SIZE)
		return (malloc_tiny_or_small(size, SMALL_CHUNCK_SIZE, &g_memory.small));
	return (malloc_large(size));
}
