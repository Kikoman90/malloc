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

void    *calloc(size_t nmemb, size_t size)
{
    void	*ptr;

    if ((ptr = malloc(nmemb * size)))
        ft_memset(ptr, 0, nmemb * size);
    return (ptr);
}