#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "malloc.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int     ft_strlen(char const *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

void	ft_putstr(char const *s)
{
	write(1, s, ft_strlen(s));
}

void	ft_putendl(char const *s)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
}

void    ft_print_unsigned_long_long(unsigned long long n)
{
    if (n > 9 && n <= ULLONG_MAX)
        ft_print_unsigned_long_long(n / 10);
    ft_putchar(n % 10 + '0');
}

char	*ft_itoa_base_seize(unsigned long long value, char *buff)
{
	unsigned long long res;
	unsigned long long j = 0;
	unsigned long long symb;
	unsigned long long stock = 0;

	j = 12;
	if (value > ULLONG_MAX)	
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
		buff[j] = stock + '0';
		j--;
	}
	while (j > 1)
	{
		if ((res % 16) >= 10)
			buff[j] = ((res % 16 - 9) + '@');	
		else
			buff[j] = ((res % 16) + '0');
		res /= 16;
		j--;
	}
	buff[0] = '0';
	buff[1] = 'x';
	buff[13] = '\0';
	return (buff);
}

void print_addr(void *addr, short ret_line)
{
    char buff[14];

    ft_putstr(ft_itoa_base_seize((unsigned long long)addr, buff));
	if (ret_line)
		ft_putstr("\n");
    return ;
}

int hexadiff(void *addr1, void *addr2)
{
    print_addr(addr1, 0);
    ft_putstr(" - ");
    print_addr(addr2, 0);
    ft_putstr(" : ");
    ft_print_unsigned_long_long(addr2 - addr1);
    ft_putstr(" octets\n");
	return (addr2 - addr1);
}

int show_zone(t_meta *list)
{
	t_meta *save;
	size_t total;

	total = 0;
	save = list;
	while (save)
	{
		if (save->used == TRUE)
			total += hexadiff(save->addr, (void *)((char *)save->addr + save->size));
		save = save->next;
	}
	ft_putstr("\n");
	return (total);
}

void print_tab(char *tab[], size_t nb_string, short print_octets)
{
	unsigned int cpt;
	
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

// mon enfant, ton code buggait donc je l'ai changé
void show_alloc_mem(void)
{
	t_memzone	*m_zone;
	short		type;
	size_t		total;

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
	print_tab((char *[2]){"Total : ", " octets"}, 2, total);
}

void show_alloc_mem_ex()
{
	//boucle par type
	//si error afficher adress de l'erreur + size et message 
	//affichier un tableau des octets de la start adress a la end adress 16 bytes/lignes
	//az = allocaded zone
	//fz = free zone 
	//nz = null zone
	//[type de zone] = byte error
	//legende 
}
/*
TINY : 0xA0000
0xA0020 - 0xA004A : 42 octets
0xA006A - 0xA00BE : 84 octets
SMALL : 0xAD000
0xAD020 - 0xADEAD : 3725 octets
LARGE : 0xB0000
0xB0020 - 0xBBEEF : 48847 octets
Total : 52698 octets
*/