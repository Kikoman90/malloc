/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 10:23:27 by fsidler           #+#    #+#             */
/*   Updated: 2019/02/04 10:23:29 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*ft_memcpy(void *dest, void *src, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
		i++;
	}
	return (dest);
}

void	*ft_memset(void *dest, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char*)dest)[i] = c;
		i++;
	}
	return (dest);
}

void	*calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (pthread_mutex_lock(&g_mutex))
		return (log_error_null("error [mutex_lock]: ", strerror(errno)));
	if ((ptr = malloc_unsafe(nmemb * size)))
		ft_memset(ptr, 0, nmemb * size);
	if (pthread_mutex_unlock(&g_mutex))
		return (log_error_null("error [mutex_unlock]: ", strerror(errno)));
	return (ptr);
}
