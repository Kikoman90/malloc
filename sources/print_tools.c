#include "malloc.h"

int		ft_strlen(char const *str)
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
