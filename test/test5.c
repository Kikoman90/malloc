#include "../includes/malloc.h"

int	main()
{
	malloc(1024);
	malloc(1024 * 32);
	malloc(1024 * 1024);
	malloc(1024 * 1024 * 16);
	malloc(1024 * 1024 * 128);
	malloc(32769);
	show_alloc_mem();
	return (0);
}