
#include "malloc.h"

char	*ft_itoa_addr(ull_64 value, char *s, ull_64 size, short print_ox)
{
	ull_64	res;
	ull_64	j;
	ull_64	symb;
	ull_64	stock;
	short	end;

	j = size - 1;
	stock = 0;
	if (value > ULLONG_MAX || size > 14)
		return (NULL);
	if (value == ULLONG_MAX)
	{
		stock = value % 10;
		value /= 10;
		j++;
	}
	res = value;
	if (stock != 0)
	{
		s[j] = stock + '0';
		j--;
	}
	end = (print_ox == 1) ? 2 : 0;
	while (j >= end)
	{
		s[j] = ((res % 16) >= 10) ? ((res % 16 - 9) + '@') : ((res % 16) + '0');
		res /= 16;
		j--;
	}
	if (print_ox)
	{
		s[0] = '0';
		s[1] = 'x';
	}
	s[size] = '\0';
	return (s);
}

void	print_addr(void *addr, short ret_line)
{
	char	buff[15];

	ft_putstr(ft_itoa_addr((ull_64)addr, buff, 14, 1));
	if (ret_line)
		ft_putstr("\n");
	return ;
}

int		hexadiff(void *addr1, void *addr2)
{
	print_addr(addr1, 0);
	ft_putstr(" - ");
	print_addr(addr2, 0);
	ft_putstr(" : ");
	ft_print_unsigned_long_long(addr2 - addr1);
	ft_putstr(" octets\n");
	return (addr2 - addr1);
}

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

void	print_tab(char *tab[], size_t nb_string, ull_64 print_octets)
{
	unsigned int	cpt;

	cpt = 0;
	if (nb_string == 2 && print_octets != -1)
	{
		ft_putstr(tab[0]);
		ft_print_unsigned_long_long(print_octets);
		ft_putendl(tab[1]);
		return ;
	}
	while (cpt < nb_string)
	{
		ft_putstr(tab[cpt]);
		cpt++;
	}
}

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

void	print_ascii(unsigned char *buff, size_t *pos)
{
	unsigned int	cpt;
	char			c;

	cpt = 0;
	while (cpt <= *pos)
	{
		if (cpt == 0)
			ft_putstr(" [ ");
		c = buff[cpt];
		if (!(c >= 0x20 && c <= 0x7e))
			c = '.';
		ft_putchar(c);
		if (cpt == *pos)
			ft_putstr(" ]\n");
		cpt++;
	}
}

void	tab_ascii(unsigned char *s, size_t *pos, size_t l_oct, unsigned char c)
{
	if (*pos != octetline)
		s[*pos] = c;
	else
	{
		s[*pos] = c;
		s[*pos + 1] = '\0';
		print_ascii(s, pos);
	}
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

void	prt_header_hexadump(short type)
{
	if (type == TINY || type == SMALL)
		ft_putstr((type == TINY) ? "\nTINY :\n" : "\nSMALL : \n");
	else
		ft_putstr("\nLARGE :\n");
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
	type = TINY;
	while (type < LARGE)
	{
		m_zone = (type == TINY) ? g_memory.tiny : g_memory.small;
		cpt = 0;
		prt_header_hexadump(type);
		while (m_zone)
		{
			total += show_octet(m_zone->meta, &cpt, 15);
			m_zone = m_zone->next;
		}
		type++;
	}
	prt_header_hexadump(type);
	cpt = 0;
	if (g_memory.large)
		total += show_octet(g_memory.large, &cpt, 15);
	print_tab((char *[2]){"Total : \033[36m", " \033[0moctets"}, 2, total);
}
