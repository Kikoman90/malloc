#include "malloc.h"

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

void    __attribute__((visibility("default"))) *calloc(size_t nmemb, size_t size)
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