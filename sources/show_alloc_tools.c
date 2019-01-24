#include "malloc.h"

void	print_addr(void *addr, short ret_line)
{
	char	buff[15];

	ft_putstr(ft_itoa_addr((ull_64)addr, buff, 14, 1));
	if (ret_line)
		ft_putstr("\n");
	return ;
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
	if (*pos != l_oct)
		s[*pos] = c;
	else
	{
		s[*pos] = c;
		s[*pos + 1] = '\0';
		print_ascii(s, pos);
	}
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
