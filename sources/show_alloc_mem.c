/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:24:31 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/04 10:24:32 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int		show_zone(t_meta *list)
{
	t_meta	*save;
	size_t	total;

	total = 0;
	save = list;
	while (save)
	{
		if (save->used)
			total += hexadiff(save->addr, \
				(void *)((char *)save->addr + save->size));
		save = save->next;
	}
	ft_putstr("\n");
	return (total);
}

void	show_alloc_mem(void)
{
	t_memzone	*m_zone;
	short		type;
	long long	total;

	total = 0;
	type = TINY;
	while (type < LARGE)
	{
		m_zone = (type == TINY) ? g_memory.tiny : g_memory.small;
		while (m_zone)
		{
			ft_putstr((type == TINY) ? "TINY : " : "SMALL : ");
			print_addr(m_zone, 1);
			total += show_zone(m_zone->meta);
			m_zone = m_zone->next;
		}
		type++;
	}
	if (g_memory.large)
	{
		ft_putstr("LARGE : ");
		print_addr(g_memory.large, 1);
		total += show_zone(g_memory.large);
	}
	print_tab((char *[2]){"Total : \033[36m", " \033[0moctets"}, 2, total);
}
