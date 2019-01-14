// 42 header 

#include "malloc.h"

/*
** put a t_meta back in the pool
void    metathrow(t_metapool *pool, t_meta *elem)
{
	elem->prev = NULL;
	elem->next = pool->pool;
	pool->pool->prev = elem;
	pool->pool = elem;
}
*/

int     destroy_meta(t_meta *meta)
{
	if (meta->prev)
		meta->prev->next = meta->next;
	if (meta->next)
		meta->next->prev = meta->prev;
	if (munmap(meta->addr, meta->size) == -1)
		return (log_error("error [destroy_meta]: ", strerror(errno))); // incomplete
	if (munmap(meta, sizeof(t_meta)) == -1)
		return (log_error("error [destroy_meta]: ", strerror(errno))); // incomplete
	return (TRUE);
}

t_meta  *insert_meta(t_metapool *pool, t_meta *meta, void *addr, size_t size)
{
	t_meta	*insert;

	if (!(insert = metadip(pool, addr, size)))
		return (NULL); // check
	if (meta->next)
		meta->next->prev = insert;
	insert->next = meta->next;
	insert->prev = meta;
	meta->next = insert;
	return (insert);
}

t_meta  *metadip(t_metapool *metapool, void *addr, size_t size)
{
	t_metapool	*pool;
	t_meta		*elem;

	if (!metapool || !metapool->pool)
	{
		if (!(pool = create_metapool(POOL_SIZE)))
			return (NULL); // check
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

t_meta  *remove_meta(t_meta *meta, t_metapool *pool)
{
	t_meta	*prev;

	prev = meta->prev;
	if (meta->prev)
        meta->prev->next = meta->next;
    if (meta->next)
        meta->next->prev = meta->prev;
	meta->prev = NULL;
    meta->next = pool->pool;
    pool->pool->prev = meta;
    pool->pool = meta;
	return (prev);
}