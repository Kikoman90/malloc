// 42 header

#include "malloc.h"

int		log_error(const char *str1, const char *str2) //... more strings ?
{
	if (MALLOC_DEBUG)
	{
        if (str1)
		    ft_putstr(str1);
		if (str2)
            ft_putstr(str2);
        ft_putchar('\n');        
		// ft_putendl(str3); "\n" !!!!!
	}
	return (0);
}

void	*log_error_null(const char *str1, const char *str2) //... more strings ?
{
	if (MALLOC_DEBUG)
	{
        if (str1)
		    ft_putstr(str1);
        if (str2)
		    ft_putstr(str2);
        ft_putchar('\n');
		// ft_putendl(str3); "\n" !!!!!
	}
	return (NULL);
}
