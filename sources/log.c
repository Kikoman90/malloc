// 42 header

#include "malloc.h"

int	log_error(const char *str1, const char *str2)
{
	if (MALLOC_DEBUG)
	{
		if (str1)
			ft_putstr_fd(2, str1);
		if (str2)
			ft_putstr_fd(2, str2);
		ft_putchar_fd(2, '\n');
	}
	return (0);
}

void	*log_error_null(const char *str1, const char *str2)
{
	if (MALLOC_DEBUG)
	{
		if (str1)
			ft_putstr_fd(2, str1);
		if (str2)
			ft_putstr_fd(2, str2);
		ft_putchar_fd(2, '\n');
	}
	return (NULL);
}

