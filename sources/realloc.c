/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:24:26 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/13 18:01:31 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	*realloc_large(t_meta *meta, size_t size)
{
	void	*ptr;

	ptr = NULL;
	if (!(size - meta->size))
		return (meta->addr);
	if (size)
	{
		if (!(ptr = ft_memcpy(malloc_unsafe(size), meta->addr, \
			(meta->size < size) ? meta->size : size)))
			return (log_error_null("error [realloc]: malloc has failed", NULL));
	}
	if (!destroy_meta(meta, &g_memory.large))
	{
		return (log_error_null(\
			"error [realloc]: destroy_meta has failed", NULL));
	}
	return (ptr);
}

static void	*extend_mem(t_memzone ***m_zone, t_meta *meta, long diff)
{
	t_meta	*insert;

	meta->size += diff;
	if (meta->next && !meta->next->used && (long)meta->next->size >= diff)
	{
		meta->next->addr = (char*)meta->next->addr + diff;
		if (!(meta->next->size -= diff))
			remove_meta(meta->next, (**m_zone)->pool);
		return (meta->addr);
	}
	if (!(insert = insert_meta(&(**m_zone)->pool, meta, \
		(void*)((char*)meta->addr + meta->size), (size_t)(-diff))))
	{
		return (log_error_null(\
			"error [realloc]: insert_meta has failed", NULL));
	}
	insert->used = FALSE;
	return (meta->addr);
}

static void	*realloc_tiny_or_small(t_memzone ***m_zone, size_t chunck_size, \
	t_meta *meta, size_t size)
{
	long	diff;
	void	*ptr;

	if (!(diff = size - meta->size))
		return (meta->addr);
	if (!size || (size <= TINY_CHUNCK_SIZE && chunck_size == SMALL_CHUNCK_SIZE)\
		|| size > chunck_size || (diff > 0 && (!meta->next || meta->next->used\
		|| meta->next->size < (size_t)diff)))
	{
		if (size && !(ptr = ft_memcpy(malloc_unsafe(size), meta->addr, \
			(meta->size < size) ? meta->size : size)))
			return (log_error_null("error [realloc]: malloc has failed", NULL));
		if (!free_elem(m_zone, chunck_size, meta))
			return (log_error_null("error [realloc]: free has failed", NULL));
		return (ptr);
	}
	return (extend_mem(m_zone, meta, diff));
}

void		*realloc(void *ptr, size_t size)
{
	t_meta		*meta;
	t_memzone	**m_zone;
	size_t		chunck_size;

	if (pthread_mutex_lock(&g_mutex))
		return (log_error_null("error [mutex_lock]: ", strerror(errno)));
	if (!ptr)
		ptr = malloc_unsafe(size);
	else if ((size = align(size)) > MAX_SIZE)
		ptr = log_error_null("error [realloc]: size is invalid", NULL);
	else if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
		ptr = log_error_null("error [realloc]: ptr is invalid", NULL);
	else
		ptr = (m_zone) ? realloc_tiny_or_small(&m_zone, chunck_size, \
			meta, size) : realloc_large(meta, size);
	if (pthread_mutex_unlock(&g_mutex))
		return (log_error_null("error [mutex_unlock]: ", strerror(errno)));
	return (ptr);
}
