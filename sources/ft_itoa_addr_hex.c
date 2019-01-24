#include "malloc.h"

ull_64	get_value(ull_64 *j, ull_64 size, ull_64 *value, ull_64 *stock)
{
	*j = size - 1;
	*stock = 0;
	if (*value > ULLONG_MAX || size > 14)
		return (-1);
	if (*value == ULLONG_MAX)
	{
		*stock = *value % 10;
		*value /= 10;
		*j = *j + 1;
	}
	return (*value);
}

char	*ft_itoa_addr(ull_64 value, char *s, ull_64 size, short print_ox)
{
	ull_64	res;
	ull_64	j;
	ull_64	stock;
	ull_64	end;

	if ((res = get_value(&j, size, &value, &stock)) == (ull_64)-1)
		return (NULL);
	if (stock != 0)
		s[j--] = stock + '0';
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
