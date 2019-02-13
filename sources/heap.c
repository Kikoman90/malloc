/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:23:50 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/13 20:07:37 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_metapool	*create_metapool(size_t nb_meta)
{
	t_metapool	*pool;
	t_meta		*tmp;
	void		*end;
	size_t		tot_size;

	tot_size = align_to_page(nb_meta * sizeof(t_meta));
	if ((pool = mmap(0, tot_size, PROT_READ | PROT_WRITE, \
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (log_error_null("error [create_metapool]: ", strerror(errno)));
	end = (void*)((char*)pool + (tot_size - align(sizeof(t_meta))));
	pool->pool = (t_meta*)(pool + 1);
	tmp = pool->pool;
	while ((size_t)tmp < (size_t)end)
	{
		tmp->next = tmp + 1;
		tmp->next->prev = tmp;
		tmp = tmp->next;
	}
	tmp->next = NULL;
	pool->size = tot_size;
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
	if (!(zone->pool = create_metapool(SIZE_POOL)))
		return (NULL);
	zone->pool->next = NULL;
	if (!(zone->meta = metadip(&zone->pool, (void*)(zone + 1), \
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

int			destroy_memzone(t_memzone *zone, t_memzone **head, size_t size)
{
	zone->meta = NULL;
	if (zone->prev)
		zone->prev->next = zone->next;
	else
		*head = zone->next;
	if (zone->next)
		zone->next->prev = zone->prev;
	if (!destroy_metapools(zone->pool))
		return (FALSE);
	zone->pool = NULL;
	if (munmap(zone, align_to_page(size)) == -1)
		return (log_error("error [destroy_memzone]:", strerror(errno)));
	return (TRUE);
}
