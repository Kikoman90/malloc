/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:24:35 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/04 10:24:36 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	dump_hexa(t_meta *save, size_t cpt, size_t *pos, unsigned char *x)
{
	void	*addr;
	char	c;

	addr = (void*)((char *)save->addr + cpt);
	if (*pos == 0)
	{
		print_addr(addr, 0);
		ft_putstr(" : { ");
	}
	c = *(char *)(x + cpt);
	if (save->used == 1)
		ft_putstr("\033[33m");
	else
		(c != 0x0) ? ft_putstr("\033[31m") : ft_putstr("\033[34m");
	if ((unsigned char)c > 0xF)
		ft_print_uc_hex((unsigned char)c);
	else
	{
		ft_putchar('0');
		ft_print_uc_hex((unsigned char)c);
	}
	ft_putstr("\033[0m");
}

int		show_octet(t_meta *list, size_t *pos, size_t octetline)
{
	t_meta			*save;
	size_t			total;
	size_t			cpt;
	unsigned char	*x;
	unsigned char	buff[octetline + 1];

	total = 0;
	save = list;
	while (save)
	{
		cpt = 0;
		*pos = (*pos == octetline) ? 0 : *pos;
		x = (unsigned char *)save->addr;
		while (cpt < save->size)
		{
			dump_hexa(save, cpt, pos, x);
			ft_putstr((*pos != octetline) ? ", " : " } ");
			tab_ascii(buff, pos, octetline, *(x + cpt));
			*pos = (*pos == octetline) ? 0 : *pos + 1;
			cpt++;
		}
		total += (save->used == TRUE) ? save->size : 0;
		save = save->next;
	}
	return (total);
}

void	prt_header_hexadump(short type, void *address)
{
	if (type == TINY || type == SMALL)
		ft_putstr((type == TINY) ? "\nTINY : " : "\nSMALL : ");
	else
		ft_putstr("\nLARGE : ");
	print_addr(address, TRUE);
	ft_putstr("Offset address |   00  01  02  03  04  05  06  ");
	ft_putstr("07  08  09  0A  0B  0C  0D  0E  0F    |      ASCII       |\n");
	ft_putstr("-----------------------------------------------------");
	ft_putstr("----------------------------------------------------\n");
}

void	show_alloc_mem_ex(void)
{
	t_memzone	*m_zone;
	short		type;
	long long	total;
	size_t		cpt;

	total = 0;
	type = TINY - 1;
	while (++type < LARGE)
	{
		m_zone = (type == TINY) ? g_memory.tiny : g_memory.small;
		cpt = 0;
		while (m_zone)
		{
			prt_header_hexadump(type, m_zone);
			total += show_octet(m_zone->meta, &cpt, 15);
			m_zone = m_zone->next;
		}
	}
	cpt = 0;
	if (g_memory.large)
	{
		prt_header_hexadump(type, g_memory.large);
		total += show_octet(g_memory.large, &cpt, 15);
	}
	print_tab((char *[2]){"Total : \033[36m", " \033[0moctets"}, 2, total);
}
