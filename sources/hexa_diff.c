/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexa_diff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:23:56 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/04 10:23:58 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int		hexadiff(void *addr1, void *addr2)
{
	print_addr(addr1, 0);
	ft_putstr(" - ");
	print_addr(addr2, 0);
	ft_putstr(" : ");
	ft_print_unsigned_long_long(addr2 - addr1);
	ft_putstr(" octets\n");
	return (addr2 - addr1);
}
