/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_padding.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:40:45 by fsidler           #+#    #+#             */
/*   Updated: 2019/01/22 15:09:58 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
#include <stdio.h>

int		main(int argc, char **argv) {
	
	(void)argc;
	(void)argv;
	char	*ptr1;
	char	*ptr2;
	
	if (!(ptr1 = (char*)malloc(1)))
	{
		printf("ptr1 malloc failed\n");
		return (0);
	}
	if (!(ptr2 = (char*)malloc(1)))
	{
		printf("ptr2 malloc failed\n");
		return (0);
	}
	printf("ptr1 = %p\nptr2 = %p\n", ptr1, ptr2);
	return (0);
}
