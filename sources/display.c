// 42 header

#include "malloc.h"

void    display_meta_mem(void *addr, size_t size, size_t nb_bytes)
{
    size_t  i;

    i = 0;
    size /= nb_bytes;
    printf("\033[0mMEMORY [\n");
    while (i < size)
    {
        if (nb_bytes == 4)
            printf("mem[%zu] = %d\n", i, ((int*)(addr))[i]);
        else if (nb_bytes == 1)
            printf("mem[%zu] = %c\n", i, ((char*)(addr))[i]);
        else if (nb_bytes == 8)
            printf("mem[%zu] = %ld\n", i, ((long*)(addr))[i]);
        //addr += nb_bytes;
        i++;
    }
    printf("]\n");
}

void    display_meta(t_meta *meta, size_t *nb_bytes, int display_mem)
{
    t_meta  *list;
    int i  = 0;

    printf("**********************_DISPLAY_META_*************************\n");
    list = meta;
    while (list)
    {
        (list->used) ? printf("\033[31mallocated memory\n") : printf("\033[32mfree memory\n");
        printf("start addr : %p\n", list->addr);
        printf("end addr : %p\n", (void *)((char *)list->addr + list->size));
        hexadiff(list->addr, (void *)((char *)list->addr + list->size));
        ft_putchar('\n');
        if (display_mem && list->used)
            display_meta_mem(list->addr, list->size, nb_bytes[i]);
        printf("-------------------------------------------------\n");
        i++;
        list = list->next;
    }
    printf("\n");
}

// 0x1f170 (allocated - color:red);
// 0x1F175 (free - color:green);