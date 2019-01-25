// header 42

#include "malloc.h"

t_mem			g_memory = {0, 0, 0};
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void	*new_alloc(t_meta *elem, t_memzone *zone, size_t size)
{
	t_meta	*insert;

	if (elem->size > size)
	{
		if (!(insert = insert_meta(zone->pool, elem, \
			(void*)((char*)elem->addr + size), elem->size - size)))
			return (NULL);
		insert->used = FALSE;
	}
	elem->size = size;
	elem->used = TRUE;
	return (elem->addr);
}

static void	*malloc_tiny_or_small(size_t size, size_t chunck_size, \
	t_memzone **m_zone)
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

static void	*malloc_large(size_t size)
{
	t_meta	*meta;

	if ((meta = mmap(0, sizeof(t_meta), PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (log_error_null("error [malloc_large]: ", strerror(errno)));
	if ((meta->addr = mmap(0, size, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		if (munmap(meta, sizeof(t_meta)) == -1)
			log_error_null("error [malloc_large]: ", strerror(errno));
		return (log_error_null("error [malloc_large]: ", strerror(errno)));
	}
	meta->size = size;
	meta->used = TRUE;
	meta->prev = NULL;
	meta->next = g_memory.large;
	if (g_memory.large)
		g_memory.large->prev = meta;
	g_memory.large = meta;
	return (meta->addr);
}

void		*malloc_unsafe(size_t size)
{
	if ((size = align(size)) > MAX_SIZE)
		return (log_error_null("error [malloc]: size is invalid", NULL));
	if (!size)
		size = 16;
	if (size <= TINY_CHUNCK_SIZE)
		return (malloc_tiny_or_small(size, TINY_CHUNCK_SIZE, &g_memory.tiny));
	if (size <= SMALL_CHUNCK_SIZE)
		return (malloc_tiny_or_small(size, SMALL_CHUNCK_SIZE, &g_memory.small));
	return (malloc_large(size));
}

void		*malloc(size_t size)
{
	void	*ptr;

	if (pthread_mutex_lock(&g_mutex))
		return (log_error_null("error [mutex_lock]: ", strerror(errno)));
	ptr = malloc_unsafe(size);
	if (pthread_mutex_unlock(&g_mutex))
		return (log_error_null("error [mutex_unlock]: ", strerror(errno)));
	return (ptr);
}
