/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmpmain.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/25 15:35:53 by fsidler           #+#    #+#             */
/*   Updated: 2019/01/25 18:11:36 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void __attribute__((destructor)) end() {
    while (getchar() != 'a') {}
}

# define N 4
# define SIZE 31

typedef struct  s_chunck
{
    size_t      a[15];
}               t_chunck; //120 bytes

// PTR TO N ARRAYS OF LENGTH SIZE
t_chunck        *g_arr[N][SIZE];
size_t          g_st[16] = {\
                    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

void            free_zone(size_t i, size_t start, size_t len)
{
    size_t  j;

    if (i < N)
    {
        j = start;
        while (j < len)
        {
            free(g_arr[i][j]);
            j++;
        }
    }
}

//g_arr[0] = 32 -> 128
//g_arr[1] = 32 -> 

int             main()
{
    size_t      i;
    size_t      j;
    
    // allocating
    i = N;
    while (i-- > 0)
    {
        j = 0;
        while (j < SIZE)
        {
            if (!(g_arr[i][j] = (t_chunck*)malloc(sizeof(t_chunck))))
                printf("\033[31marr[%zu][%zu] malloc failed\033[0m\n", i, j);
            else
                ft_memcpy(&g_arr[i][j]->a[0], &g_st[0], 128);
            j++;   
        }
    }
    printf("\033[32msizeof(t_chunck) = %zu\033[0m\n", sizeof(t_chunck));
    printf("\033[31mallocating\033[0m\n");
    show_alloc_mem();
    hexadiff((char*)&g_memory.tiny->meta, (char*)(&g_memory.tiny->meta) + TINY_CHUNCK_SIZE * MAX_ALLOC);

    // freeing zones
    //  - freeing some zones
    printf("\033[31mfree(\033[0m\n");
    free_zone(1, 0, SIZE);
    free_zone(2, 0, SIZE);
    free_zone(0, 0, SIZE);
    show_alloc_mem();
    //show_alloc_mem_ex();
    printf("\033[31marr[0][*])\033[0m\n");

    //  - freeing last zone
    printf("\033[31mfree(\033[0m\n");
    free_zone(3, 0, SIZE);
    show_alloc_mem_ex();
    //show_alloc_mem_ex();
    printf("\033[31marr[3][*])\033[0m\n");
    return (0);
}
