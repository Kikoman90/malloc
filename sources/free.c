// header 42

#include "malloc.h"

t_meta  *merge_chuncks(t_meta *first, t_meta *second, t_metapool *pool)
{
    first->size += second->size;
    if (second->next)
        second->next->prev = first;
    first->next = second->next;
    metathrow(pool, second);
    return (first);
}

int     coalesce_chuncks(t_memzone ***m_zone, size_t chunck_size, t_meta *elem)
{
    t_meta  *tmp;

    elem->used = 0;
    if (elem->next && !elem->next->used)
        elem = merge_chuncks(elem, elem->next, (**m_zone)->pool);
    if (elem->prev && !elem->prev->used)
        elem = merge_chuncks(elem->prev, elem, (**m_zone)->pool);
    if (!elem->prev && !elem->next)
    {
        if (munmap((**m_zone)->pool, (**m_zone)->pool->size) == -1)
            return (FALSE);
        (**m_zone)->pool = NULL;
        if (munmap(**m_zone, chunck_size * MAX_ALLOC) == -1)
            return (FALSE);
        **m_zone = NULL;
        printf("munmap success\n");
    }
    return (TRUE);
}

int     free_elem(void *ptr, t_memzone ***m_zone, t_meta *elem, \
    size_t chunck_size)
{
    while (elem)
    {
        if (ptr == elem->addr)
        {
            if (m_zone)
            {
                elem->used = 0;
                return (coalesce_chuncks(m_zone, chunck_size, elem));
            }
            else
            {
                // free_large(elem);
                if (munmap(elem->addr, elem->size) == -1)
                {
                    // printf(error munmap);
                    return (FALSE);
                }
                if (elem->prev)
                    elem->prev->next = elem->next;
                if (elem->next)
                    elem->next->prev = elem->prev;
                if (munmap(elem, sizeof(t_meta)) == -1)
                {
                    // printf(error munmap);
                    return (FALSE);
                }
                return (TRUE);
            }
        }
        else if (ptr > elem->addr && (char*)ptr < (char*)elem->addr + elem->size)
            return (FALSE);
        elem = elem->next;
    }
    return (FALSE);
}

t_meta  *ptr_in_zones(void *ptr, t_memzone ***m_zone, size_t *chunck_size)
{
    short       type;
    void        *start_addr;
    t_meta      *meta;
    
    type = TINY;
    while (type < LARGE)
    {
        *chunck_size = (type == TINY) ? TINY_CHUNCK_SIZE : SMALL_CHUNCK_SIZE;
        *m_zone = (type == TINY) ? &g_memory.tiny : &g_memory.small;
        while (**m_zone)
        {
            start_addr = (void*)(**m_zone + 1);
            if (ptr >= start_addr && ptr < start_addr + MAX_ALLOC * *chunck_size)
                return ((**m_zone)->meta);
            **m_zone = (**m_zone)->next;
        }
        type++;
    }
    meta = g_memory.large;
    while (meta)
    {
        if (ptr == meta->addr)
            return (meta);
        meta = meta->next;
    }
    return (NULL);
}

void    myfree(void *ptr)
{
    t_meta      *meta;
    t_memzone   **m_zone;
    size_t      chunck_size;

    m_zone = NULL;
    chunck_size = 0;
    if (!ptr)
        ;//printf("error: ptr is null / free()"\n);
    if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
        ;//printf("error: ptr out of zones or invalid free() // Pointer non allouee !\n");
    else if (!free_elem(ptr, &m_zone, meta, chunck_size))
        ;//printf("error: invalid ptr free() // Pointer non allouee !\n");
    else
        ;//printf("free a bien fonctionnee\n");
}