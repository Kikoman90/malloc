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
t_meta			*pool_pick(void)
{

}

t_memzone		*create_memzone(size_t chunck_size)
{
	t_memzone	*zone;
	t_meta		*free_elem;

	if ((zone == mmap(0, chunck_size * MAX_ALLOC, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		// if (MALLOC_DEBUG)
		//		print err
		//		return (log_error_null(ft_strjoin("error (mmap): ", strerror(errno)), 1));
		return (NULL);
	}
	zone->alloc = NULL;
	zone->prev = NULL;
	free_elem = pick_pool();
	free_elem->addr = zone + sizeof(t_memzone);
	free_elem->size = chunck_size;
	free_elem->prev = NULL;
	while ((free_elem->addr) + (free_elem->size) < zone + (chunck_size * MAX_ALLOC)) {
		free_elem->next = pick_pool();
		free_elem->next->prev = free_elem;
		free_elem = free_elem->next;
		free_elem->addr = free_elem->prev->addr + chunck_size;
		free_elem->size = chunck_size;
	}
	free_elem->next = NULL;
	return (zone);
}

void			*malloc_tiny_small(size_t size, size_t chunck_size, t_memzone **m_zone)
{
	t_memzone	*zone;

	zone = m_zone;
	while (zone && !zone->free)
		zone = zone->next;
	// offset is sizeof(t_memzone)
	if (!zone && !(zone = create_memzone(chunck_size)))
		if ((zone == mmap(0, chunck_size * MAX_ALLOC, PROT_READ | PROT_WRITE, \
			MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		{
			// if (MALLOC_DEBUG)
			//		print err
					return (log_error_null(ft_strjoin("error (mmap): ", strerror(errno)), 1));
			return (NULL);
		}
			return (NULL);
	}
	find_free_space(zone->free);

	return (NULL);
}

void			*malloc_large(size_t size)
{

	return (NULL);
}

static t_meta	*create_metapool(size_t nb_elem)
{
	t_meta	*pool;

	if (!(pool = mmap(0, sizeof(t_meta) * nb_elem, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		// if (MALLOC_DEBUG)

		//		print err;
		return (NULL);
	}
	return (pool);
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
	if (!g_memory.metapool && !(g_memory.metapool = create_metapool(61440)))
		return (NULL);
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
