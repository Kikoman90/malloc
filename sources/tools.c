/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:24:45 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/04 10:24:47 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	ft_putendl(char const *s)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
}

void	ft_print_unsigned_long_long(t_ull_64 n)
{
	if (n > 9 && n <= ULLONG_MAX)
		ft_print_unsigned_long_long(n / 10);
	ft_putchar(n % 10 + '0');
}

void	ft_print_uc_hex(unsigned char n)
{
	if ((n > 15 && n <= 255))
		ft_print_uc_hex(n / 16);
	if ((n % 16) >= 10)
		ft_putchar((n % 16 - 9) + '@');
	else
		ft_putchar(n % 16 + '0');
}

size_t	align(size_t size)
{
	if (ALIGNMENT <= 1)
		return (size);
	return (((size + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT);
}

size_t	align_to_page(size_t size)
{
	size_t	pg_size;

	pg_size = getpagesize();
	return (((size + pg_size - 1) / pg_size) * pg_size);
}
