// header 42

#include "malloc.h"

t_mem			g_memory = {NULL, NULL, NULL, NULL};
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;
/*
void			ft_puterror(const char *err, const char *errno, )
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

static t_meta	*create_metapool(size_t nb_elem)
{
	t_meta	*pool;
	t_meta	*tmp;
	//void	*end;
	t_meta	*end;

	if ((pool = mmap(0, sizeof(t_meta) * nb_elem, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		// if (DEBUG_MALLOC)
		// print err
		return (NULL);
	}
	end = pool + (nb_elem - 1);
	printf("addr end = %p\n", end);
	pool->prev = NULL;
	tmp = pool;
	nb_elem--;
	while (tmp < end)
	{
		tmp->next = tmp + 1;
		printf("tmp->next = %p\n", tmp->next);
		tmp->next->prev = tmp;
		printf("end = %p\n", end);
		tmp = tmp->next; 
		printf("cpt = %zu\n", --nb_elem);
	}
	printf("loop\n");
	tmp->next = NULL;
	return (pool);
}

int				metathrow(t_meta *elem)
{
	elem->prev = NULL;
	g_memory.metapool->prev = elem;
	elem->next = g_memory.metapool;
	g_memory.metapool = elem;
	return (1);
}

t_meta			*metadip(void)
{
	t_meta	*mt;

	if (!g_memory.metapool)
	{
		if (!(mt = create_metapool(4096)))
			return (NULL);
		g_memory.metapool = mt;
	}
	mt = g_memory.metapool;
	g_memory.metapool->next->prev = NULL;
	g_memory.metapool = g_memory.metapool->next;
	// if metapool is empty -> munmap();
	return (mt);
}

t_memzone		*create_memzone(size_t chunck_size)
{
	t_memzone	*zone;
	t_meta		*free_elem;

	printf("mma\n");
	if ((zone = mmap(0, chunck_size * MAX_ALLOC, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		// if (MALLOC_DEBUG)
		//		print err
		//		return (log_error_null(ft_strjoin("error (mmap): ", strerror(errno)), 1));
		return (NULL);
	}
	zone->alloc = NULL;
	zone->prev = NULL;
	free_elem = metadip();
	printf("METADIP\n");
	free_elem->addr = zone + sizeof(t_memzone);
	free_elem->size = chunck_size;
	free_elem->prev = NULL;
	printf("addr = %p\naddr+size = %p\n", free_elem->addr, free_elem->addr + free_elem->size);
	while (free_elem->addr + free_elem->size < (void*)zone + chunck_size * MAX_ALLOC) {
		free_elem->next = metadip();
		free_elem->next->prev = free_elem;
		free_elem = free_elem->next;
		free_elem->addr = free_elem->prev->addr + chunck_size;
		free_elem->size = chunck_size;
	}
	free_elem->next = NULL;
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
	printf("zone is null\n");
	if (!(zone = create_memzone(chunck_size)))
		return (NULL);
	printf("create memzone\n");
	zone->next = *m_zone;
	*m_zone = zone;
	return (new_alloc(&(*m_zone)->free, &(*m_zone)->alloc, size));
}

void			*malloc_large(size_t size)
{
	(void)size;
	return (NULL);
}

// 2 mmap
// 1 pour la pool
// 1 pour la data
// 1 debug zone ? for error messages and other stuff
void			*mymalloc(size_t size)
{
	if (!size || size > LARGE_CHUNCK_SIZE)
	{
		// if (MALLOC_DEBUG)
		//	ft_putendl_fd(2, "error: size is invalid (malloc)");
		return (NULL);
	}
	//if (!g_memory.metapool && !(g_memory.metapool = create_metapool(61440)))
	//	return (NULL);
	if (size <= TINY_CHUNCK_SIZE)
		return (malloc_tiny_or_small(size, TINY_CHUNCK_SIZE, &g_memory.tiny));
	else if (size <= SMALL_CHUNCK_SIZE)
		return (malloc_tiny_or_small(size, SMALL_CHUNCK_SIZE, &g_memory.small));
	return (malloc_large(size));
	/*
	if ((mem = mmap(0, size + 1, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	return (ptr);*/
}
