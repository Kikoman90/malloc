#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

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

void print_addr(void *addr)
{
    char buff[14];

    ft_putstr(ft_itoa_base_seize((unsigned long long)addr, buff));
    return ;
}

void hexadiff(void *addr1, void *addr2)
{
    print_addr(addr1);
    ft_putstr(" - ");
    print_addr(addr2);
    ft_putstr(" : ");
    ft_print_unsigned_long_long(addr2 - addr1);
    ft_putstr(" octets");
    ft_putchar('\n');
}
