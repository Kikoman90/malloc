// header 42

#include "malloc.h"

int				free_elem(t_memzone ***m_zone, size_t chunck_size, \
	t_meta *elem)
{
	elem->used = 0;
	if (elem->next && !elem->next->used)
	{
		elem->size += elem->next->size;
		remove_meta(elem->next, (**m_zone)->pool);
	}
	if (elem->prev && !elem->prev->used)
	{
		elem->prev->size += elem->size;
		elem = remove_meta(elem, (**m_zone)->pool);
	}
	if (!elem->prev && !elem->next)
	{
		if (!destroy_memzone(**m_zone, chunck_size * MAX_ALLOC))
			return (FALSE);
		**m_zone = NULL;
	}
	return (TRUE);
}

static t_meta	*search_meta(void *ptr, t_meta *meta)
{
	while (meta)
	{
		if (ptr == meta->addr)
			return (meta);
		else if (ptr > meta->addr && \
				(char*)ptr < (char*)meta->addr + meta->size)
			return (NULL);
		meta = meta->next;
	}
	return (NULL);
}

t_meta			*ptr_in_zones(void *ptr, t_memzone ***m_zone, \
	size_t *chunck_size)
{
	void		*zone_addr;
	short		type;
	t_memzone	*tmp_zone;
	size_t		zone_size;

	type = TINY;
	while (type < LARGE)
	{
		*m_zone = (type == TINY) ? &g_memory.tiny : &g_memory.small;
		*chunck_size = (type == TINY) ? TINY_CHUNCK_SIZE : SMALL_CHUNCK_SIZE;
		zone_size = align_to_page(MAX_ALLOC * *chunck_size);
		tmp_zone = **m_zone;
		while (tmp_zone)
		{
			zone_addr = (void*)(tmp_zone + 1);
			if (ptr >= zone_addr && ptr < zone_addr + zone_size)
				return (search_meta(ptr, tmp_zone->meta));
			*m_zone = &tmp_zone->next;
			tmp_zone = tmp_zone->next;
		}
		type++;
	}
	*m_zone = NULL;
	return (search_meta(ptr, g_memory.large));
}

void			free(void *ptr)
{
	t_meta		*meta;
	t_memzone	**m_zone;
	size_t		chunck_size;

	if (pthread_mutex_lock(&g_mutex))
	{
		log_error("error [mutex_lock]: ", strerror(errno));
		return ;
	}
	if (!ptr)
		log_error("error [free]: null pointer", NULL);
	else
	{
		if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
			log_error("error [free]: invalid pointer", NULL);
		else
		{
			if (!m_zone && !destroy_meta(meta, &g_memory.large))
				log_error("error [free]", NULL);
			else if (m_zone && !free_elem(&m_zone, chunck_size, meta))
				log_error("error [free]", NULL);
		}
	}
	if (pthread_mutex_unlock(&g_mutex))
		log_error("error [mutex_unlock]: ", strerror(errno));
}
