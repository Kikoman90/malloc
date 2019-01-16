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

char	*ft_itoa_base_seize(unsigned long long value, char *buff, unsigned long long size, short print_Ox)
{
	unsigned long long res;
	unsigned long long j = 0;
	unsigned long long symb;
	unsigned long long stock = 0;
	short end;

	j = size -1;
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
		buff[j] = stock + '0';
		j--;
	}
	end = (print_Ox == 1) ? 2 : 0;
	while (j >= end)
	{
		if ((res % 16) >= 10)
			buff[j] = ((res % 16 - 9) + '@');	
		else
			buff[j] = ((res % 16) + '0');
		res /= 16;
		j--;
	}
	if (print_Ox) 
	{
		buff[0] = '0';
		buff[1] = 'x';
	}
	//printf("toto = %s\n", buff);
	buff[size] = '\0';
	return (buff);
}

void print_addr(void *addr, short ret_line)
{
    char buff[15];

    ft_putstr(ft_itoa_base_seize((unsigned long long)addr, buff, 14, 1));
	if (ret_line)
		ft_putstr("\n");
    return ;
}

void print_hex_octet(void *addr)
{
/*	char val;

	val = &addr;
	printf("val = %d\n", (char)addr);*/
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
		if (save->used)
			total += hexadiff(save->addr, (void *)((char *)save->addr + save->size));
		save = save->next;
	}
	ft_putstr("\n");
	return (total);
}

void print_tab(char *tab[], size_t nb_string, long long print_octets)
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

int show_octet(t_meta *list, size_t *pos, size_t octetline)
{
	t_meta *save;
	size_t total;
	size_t cpt;
	void *addr;

	total = 0;
	save = list;
	while (save)
	{
		cpt = 0;
		*pos = (*pos == octetline) ? 0 : *pos;
		while (cpt < save->size)
		{
			addr = (void *)((char *)save->addr + cpt);
			if (*pos == 0)
			{
				print_addr(addr,0);
				ft_putstr(" : { ");
			}
			//ft_itoa_base_seize((unsigned long long)&addr, (char[3]){}, 2, 0)
			//print_hex_octet((void *)255);
			print_tab((char *[2]){(save->used == 1) ? "\033[33maz\033[0m" : "\033[32mfz\033[0m", (*pos != octetline) ? ", " : " }\n"}, 2, -1);
			/*ft_putstr((addr != 0) ? "\033[33" : "\033[32");
			print_hex_octet(addr);
			print_tab((char *[2]){"\033[0m", (*pos != octetline) ? " " : " }"}, 2, 0);*/
			*pos = (*pos == octetline) ? 0 : *pos + 1;
			cpt++;
		}
		if (save->used == TRUE)
			total += save->size;
		save = save->next;
	}
	ft_putstr("\n");
	return (total);
}
// mon enfant, ton code buggait donc je l'ai changé
void show_alloc_mem(void)
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

	//boucle par type
	//si error afficher adress de l'erreur + size et message 
	//affichier un tableau des octets de la start adress a la end adress 16 bytes/lignes
	//az = allocaded zone
	//fz = free zone 
	//nz = null zone
	//[type de zone] = byte error
	//legende 

/*void show_alloc_mem_ex(void)
{
	t_memzone	*m_zone;
	short		type;
	long long	total;
	size_t		cpt;

	total = 0;
	type = TINY;
	while (type < LARGE)
	{
		printf("ki c ka tou kc ?\n");
		m_zone = (type == TINY) ? g_memory.tiny : g_memory.small;
		if (m_zone)
			printf("lfdsafs\n");
		cpt = 0;
		ft_putstr((type == TINY) ? "TINY :\n" : "SMALL : \n");
		sleep(1);
		while (m_zone)
		{
			printf("loool\n");
			total += show_octet(m_zone->meta, &cpt, (type == TINY) ? 15 : 31);
			m_zone = m_zone->next;
		}
		type++;
	}
	if (g_memory.large)
	{
		cpt = 0;
		ft_putstr("LARGE :\n");
		sleep(1);
		total += show_octet(g_memory.large, &cpt, 31);
	}
	print_tab((char *[2]){"Total : \033[36m", " \033[0moctets"}, 2, total);
}*/

void show_alloc_mem_ex(void)
{
	t_memzone	*m_zone;
	short		type;
	long long	total;
	size_t		cpt;

	total = 0;
	type = TINY;
	while (type < LARGE)
	{
		printf("c casse ?\n");
		m_zone = (type == TINY) ? g_memory.tiny : g_memory.small;
		if (m_zone)
			printf("lfdsafs\n");
		cpt = 0;
		ft_putstr((type == TINY) ? "TINY :\n" : "SMALL : \n");
		ft_putstr("Offset addr(hexa)");
		sleep(1);
		while (m_zone)
		{
			printf("loool\n");
			total += show_octet(m_zone->meta, &cpt,  15);
			m_zone = m_zone->next;
		}
		type++;
	}
	if (g_memory.large)
	{
		cpt = 0;
		ft_putstr("LARGE :\n");
		sleep(1);
		total += show_octet(g_memory.large, &cpt, 15);
	}
	print_tab((char *[2]){"Total : \033[36m", " \033[0moctets"}, 2, total);
	/*char buff[3];
	printf("ft_itoa_base_seize de 128= %s\n", ft_itoa_base_seize(255,buff, 2, 0));*/
}

/*
TINY : 0xA0000 - 0xA0060

0xA0020: {az, az, az, az, nz, nz}
0xA0030: {az, az, az, az, nz, nz}
0xA0040: {az, az, az, az, nz, nz}
0xA0050: {az, az, az, az, nz, nz}
0xA0060: {az, az, az, az, nz, nz}




0xA004A : 42 octets
//0xA006A - 0xA00BE : 84 octets
SMALL : 0xAD000
0xAD020 - 0xADEAD : 3725 octets
LARGE : 0xB0000
0xB0020 - 0xBBEEF : 48847 octets
Total : 52698 octets
*/