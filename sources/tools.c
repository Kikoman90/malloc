// 42 header

#include "malloc.h"

int     ft_strlen(char const *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void    ft_putchar_fd(short fd, char c)
{
    write(fd, &c, 1);
}

void	ft_putstr(char const *s)
{
	write(1, s, ft_strlen(s));
}

void    ft_putstr_fd(short fd, char const *str)
{
    write(fd, str, ft_strlen(str));
}

void	ft_putendl(char const *s)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
}

void    ft_print_unsigned_long_long(ull_64 n)
{
    if (n > 9 && n <= ULLONG_MAX)
        ft_print_unsigned_long_long(n / 10);
    ft_putchar(n % 10 + '0');
}

void    ft_print_uc_hex(unsigned char n)
{
    if ((n > 15 && n <= 255))
        ft_print_uc_hex(n / 16);
	if ((n % 16) >= 10)
		ft_putchar((n % 16 - 9)  + '@');
	else 
    	ft_putchar(n % 16 + '0');
}

size_t  align(size_t size)
{
	//static size_t shift = get_align_shift();
	return (((size + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);
}

size_t  align_to_page(size_t size)
{
	size_t	pg_size;

	pg_size = getpagesize();
	return (((size + pg_size - 1) / pg_size) * pg_size);
}