/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:24:15 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/13 20:01:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int		destroy_meta(t_meta *meta, t_meta **head)
{
	if (meta->prev)
		meta->prev->next = meta->next;
	else
		*head = meta->next;
	if (meta->next)
		meta->next->prev = meta->prev;
	if (munmap(meta->addr, meta->size) == -1)
		return (log_error("error [destroy_meta]: ", strerror(errno)));
	if (munmap(meta, sizeof(t_meta)) == -1)
		return (log_error("error [destroy_meta]: ", strerror(errno)));
	return (TRUE);
}

t_meta	*insert_meta(t_metapool **pool, t_meta *meta, void *addr, size_t size)
{
	t_meta	*insert;

	if (!(insert = metadip(pool, addr, size)))
		return (NULL);
	if (meta->next)
		meta->next->prev = insert;
	insert->next = meta->next;
	insert->prev = meta;
	meta->next = insert;
	return (insert);
}

t_meta	*metadip(t_metapool **metapool, void *addr, size_t size)
{
	t_metapool	*pool;
	t_meta		*elem;

	if (!metapool)
		return (NULL);
	if (!(*metapool) || !((*metapool)->pool))
	{
		if (!(pool = create_metapool(SIZE_POOL)))
			return (NULL);
		pool->next = *metapool;
		*metapool = pool;
	}
	pool = *metapool;
	elem = pool->pool;
	if (elem->next)
		elem->next->prev = NULL;
	pool->pool = elem->next;
	elem->addr = addr;
	elem->size = size;
	return (elem);
}

t_meta	*remove_meta(t_meta *meta, t_metapool *pool)
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
