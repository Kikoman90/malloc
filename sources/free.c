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
    if (*m_zone)
    {
        elem->used = 0;
        return (coalesce_chuncks(m_zone, chunck_size, elem));
    }
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
    printf("elem->addr = %p/elem->size = %zu\n", elem->addr, elem->size);
    return (TRUE);
}

t_meta  *search_meta(void *ptr, t_meta *meta)
{
    while (meta)
    {
        if (ptr == meta->addr)
            return (meta);
        else if (ptr > meta->addr && \
            (char*)ptr < (char*)meta->addr + meta->size)
            return (NULL);
        meta = meta->next;
    }
    return (NULL);
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
        while (**m_zone)
        {
            start_addr = (void*)(**m_zone + 1);
            if (ptr >= start_addr && ptr < start_addr + MAX_ALLOC * *chunck_size)
                return (search_meta(ptr, (**m_zone)->meta));
            **m_zone = (**m_zone)->next;
        }
        (*m_zone)++;
        type++;
    }
    *m_zone = NULL;
    return (search_meta(ptr, g_memory.large));
}

void    myfree(void *ptr)
{
    t_meta      *meta;
    t_memzone   **m_zone;
    size_t      chunck_size;

    m_zone = &g_memory.tiny;
    if (!ptr)
        ;//printf("error: ptr is null / free()"\n);
    if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
        ;//printf("error: invalid ptr free() // Pointer non allouee !\n");
    else if (!free_elem(ptr, &m_zone, meta, chunck_size))
        ;//printf("error: invalid ptr free() // Pointer non allouee !\n");
}