// 42 header

#include "malloc.h"

t_metapool	*create_metapool(size_t nb_meta)
{
	t_metapool	*pool;
	t_meta		*tmp;
	void		*end;

	if ((pool = mmap(0, sizeof(t_meta) * nb_meta, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (log_error_null("error [create_metapool]:", strerror(errno)));
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

t_memzone	*create_memzone(size_t chunck_size)
{
	t_memzone	*zone;
	size_t		size;

	size = align_to_page(chunck_size * MAX_ALLOC);
	if ((zone = mmap(0, size, PROT_READ | PROT_WRITE, \
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (log_error_null("error [create_memzone]:", strerror(errno)));
	if (!(zone->pool = create_metapool(4096)))
		return (NULL);
	zone->pool->next = NULL;
	if (!(zone->meta = metadip(zone->pool, (void*)(zone + 1), \
		size - sizeof(t_memzone))))
		return (NULL);
	zone->meta->next = NULL;
	zone->prev = NULL;
	return (zone);
}

int			destroy_metapools(t_metapool *pool)
{
	t_metapool	*tmp;

	while (pool)
	{
		tmp = pool;
		pool = pool->next;
		if (munmap(tmp, tmp->size) == -1)
			return (log_error("error [destroy_metapools]:", strerror(errno)));
	}
	return (TRUE);
}

int			destroy_memzone(t_memzone *zone, size_t size)
{
	zone->meta = NULL;
	if (zone->prev)
		zone->prev->next = zone->next;
	if (zone->next)
		zone->next->prev = zone->prev;
	if (!destroy_metapools(zone->pool))
		return (FALSE);
	zone->pool = NULL;
	if (munmap(zone, align_to_page(size)) == -1)
		return (log_error("error [destroy_memzone]:", strerror(errno)));
	return (TRUE);
}
