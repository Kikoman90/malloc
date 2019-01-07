/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapalm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 16:29:07 by fsidler           #+#    #+#             */
/*   Updated: 2019/01/07 16:30:21 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/resource.h>

int main(int argc, char **argv) {

	int		nb;
	size_t	size;
	clock_t	begin, end;
	void	*addr;
	int		err;
	struct rlimit	rl;

	addr = NULL;
	if ((nb = argc) < 2) {
		printf("usage: ./mapalm [alloc_size_in_bytes, ...]\n");
		return (0);
	}
	
	printf("sizeof(size_t) = %zu\n", sizeof(size_t));
	printf("sizeof(long) = %zu\nsizeof(long long) = %zu\n\n", sizeof(long), sizeof(long long));
	if (getrlimit(RLIMIT_AS, &rl) == -1)
		printf("getrlimit failed\n\n");
	else
		printf("soft limit = %.3Le bytes\nhard limit = %.3Le bytes\n\n", (long double)rl.rlim_cur, (long double)rl.rlim_max);

	while (nb-- > 1) {
		err = 0;
		size = (size_t)atol(argv[nb]);
		printf("MMAP(%zu):\n", size);
		if (size ==  0)// || size > UINT_MAX)
			printf("invalid size\n");
		else {
			begin = clock();
			if ((addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
				err = 1;
				printf("failed\n");
			}
			else
				printf("success: %p\n", addr);
			end = clock();
			printf("time: %f\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC);
			if (err == 0) {
				printf("MUNMAP(%p):\n", addr);
				begin = clock();
				if (munmap(addr, size) == -1)
					printf("failed\n");
				else
					printf("success\n");
				end = clock();
				printf("time = %f\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC);
			}
			printf("\n");
		}
	}
	return (0);
}
