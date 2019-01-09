// header 42

#include "malloc.h"

t_mem			g_memory = {NULL, NULL, NULL};
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
void			ft_puterror(const char *err, const char *errno, ...)
{
	char	*err;

	if (log) {
		//err = ft_strjoin("\033[31m;
		write (2, "\033[31merror", )
		write(2, err, ft_strlen(err));
		free(err);
	}
}

void			*log_error_null(const char *log, char free)
{
	if (MALLOC_DEBUG)
	{
		ft_puterror(log);
		if (log && free)
			free(log);
	}
	return (NULL);
}
*/

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
	end = (void*)(pool + sizeof(t_meta) * (nb_elem - 1));
	printf("start addr = %p\nend address = %p\n", (void*)pool, end);
	tmp = pool->pool;
	while ((void*)tmp < end)
	{
		printf("ok\n");
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
int				metathrow(t_metapool *pool, t_meta *elem)
{
	elem->prev = NULL;
	elem->next = pool->pool;
	pool->pool->prev = elem;
	pool->pool = elem;
	return (1);
}

/*
** grab a t_meta from the pool 
** NECESARRY TO MUNMAP POOLS AT A CERTAIN POINT -> needs to store size somewhere
** (#define POOL_SIZE 4096) ?
*/
t_meta			*metadip(t_metapool **metapool, void *addr, size_t size)
{
	t_metapool	*pool;
	t_meta		*new_meta;

	if (!*metapool || !(*metapool)->pool)
	{
		if (!(pool = create_metapool(4096)))
			return (NULL);
		pool->next = *metapool;
		*metapool = pool;
	}
	pool = *metapool;
	new_meta = pool->pool;
	new_meta->next->prev = NULL;
	pool->pool = new_meta->next;
	new_meta->addr = addr;
	new_meta->size = size;
	return (new_meta);
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
	zone->pool = create_metapool(3);
	zone->alloc = NULL;
	zone->free = metadip(&zone->pool, (void*)(zone + 1), (chunck_size * MAX_ALLOC) - sizeof(t_memzone));
	zone->free->next = NULL;
	zone->prev = NULL;
	return (zone);
}

//(*free_elem)->next = g_memory.metapool;
//g_memory.metapool = *free_elem;
void			*new_alloc(t_meta **free_elem, t_meta **alloc, size_t size)
{
	if ((*free_elem)->next)
		(*free_elem)->next->prev = (*free_elem)->prev;
	if ((*free_elem)->prev)
	{
		(*free_elem)->prev->next = (*free_elem)->next;
		(*free_elem)->prev = NULL;
	}
	(*alloc)->prev = *free_elem;
	(*free_elem)->next = *alloc;
	*alloc = *free_elem;
	(*alloc)->size = size;
	return ((*alloc)->addr);
}

void			*malloc_tiny_or_small(size_t size, size_t chunck_size, t_memzone **m_zone)
{
	t_memzone	*zone;
	t_meta		*free_elem;

	zone = *m_zone;
	while (zone)
	{
		free_elem = zone->free;
		while (free_elem)
		{
			if (free_elem->size >= size)
				return (new_alloc(&free_elem, &zone->alloc, size));
			free_elem = free_elem->next;
		}
		zone = zone->next;
	}
	if (!(zone = create_memzone(chunck_size)))
		return (NULL);
	zone->next = *m_zone;
	*m_zone = zone;
	return (new_alloc(&zone->free, &zone->alloc, size));
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
