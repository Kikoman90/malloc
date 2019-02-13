/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_addr_hex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:23:46 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/04 10:23:46 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_ull_64	get_value(t_ull_64 *j, t_ull_64 size, t_ull_64 *value, \
	t_ull_64 *stock)
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

char		*ft_itoa_addr(t_ull_64 value, char *s, t_ull_64 size, \
	short print_ox)
{
	t_ull_64	res;
	t_ull_64	j;
	t_ull_64	stock;
	t_ull_64	end;

	if ((res = get_value(&j, size, &value, &stock)) == (t_ull_64)-1)
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
