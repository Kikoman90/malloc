#include "malloc.h"
/*
void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;
	char	*buff;

	buff = (char *)b;
	i = 0;
	while (i < len)
	{
		buff[i] = c;
		i++;
	}
	return (buff);
}

void    *calloc(size_t nmemb, size_t size)
{
    void *ptr;

    if (nmemb == 0)
        return (NULL);
    if (ptr = mymalloc(nmemb * size) != NULL)
        ft_memset(ptr, 0, (nmemb * size));
    return (ptr);
}#include "malloc.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;
	char	*buff;

	buff = (char *)b;
	i = 0;
	while (i < len)
	{
		buff[i] = c;
		i++;
	}
	return (buff);
}
*/
void    *calloc(size_t nmemb, size_t size)
{
    /*void *ptr;

    if (nmemb == 0)
        return (NULL);
    if (ptr = mymalloc(nmemb * size) != NULL)
        ft_memset(ptr, 0, (nmemb * size));
    return (ptr);*/
	return (NULL);
}