#include "../includes/malloc.h"

int     move_ptr_to_pool(void *ptr)
{
    return (TRUE);
}

int     manage_free_elem(short type, void *ptr)
{
    t_memzone *save_g;
    t_meta *save_meta;

    save_meta = NULL;
    save_g = g_memory->tiny;
    if (*ptr == NULL || type == -1)
        return (FALSE);
    save_g = (type == SMALL) ? g_memory->small : save_g;
    save_g = (type == LARGE) ? g_memory->large : save_g;
    while (save_g->next)
    {
        save_meta = save_g->alloc;
        while (save_meta->next)
        {
            if (save_meta->addr == &ptr)
                return(move_ptr_to_pool(save_meta));
            save_meta = save_meta->next;
        }
        save_g = save->next;
    }
    return (FALSE);
}

int search_ptr(void *ptr)
{
    short i;
    int chunck_size;
    void *adrr;
    
    i = 0;
    addr = NULL;
    chunck_size = TINY_CHUNCK_SIZE;
    while (i < 3)
    {
        if (i == TINY)
            start_addr = (void*)g_memory->tiny + sizeof(t_memzone);
        else if (i == SMALL)
            start_addr = (void*)g_memory->small + sizeof(t_memzone);
        else if (i == LARGE)
            start_addr = (void*)g_memory->large + sizeof(t_memzone);
        if (ptr >= start_addr && ptr <= start_addr + MAX_ALLOC * chunck_size)
            return (i);
        chunck_size = (i == 0) ? SMALL_CHUNCK_SIZE : LARGE_CHUNCK_SIZE
        i++;
    }
    return (OUTZONE);
}

void    my_free(void *ptr)
{
    if (manage_free_elem(search_ptr(ptr), ptr) == FALSE)
        printf("error: free() // Pointer non allouee !\n");
    else
        printf("free a bien fonctionnee\n");
    return ;
}