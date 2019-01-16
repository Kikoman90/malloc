// header 42

#include "malloc.h"

/*
t_meta  *merge_chuncks(t_meta *first, t_meta *second, t_metapool *pool)
{
    first->size += second->size;
    if (second->next)
        second->next->prev = first;
    first->next = second->next;
    metathrow(pool, second);
    return (first);
}
*/

int             free_elem(t_memzone ***m_zone, size_t chunck_size, \
    t_meta *elem)
{
    t_meta  *tmp;

    elem->used = 0;
    if (elem->next && !elem->next->used)
    {
        elem->size += elem->next->size;
        remove_meta(elem->next, (**m_zone)->pool);
        // elem = merge_chuncks(elem, elem->next, (**m_zone)->pool);
    }
    if (elem->prev && !elem->prev->used)
    {
        elem->prev->size += elem->size;
        elem = remove_meta(elem, (**m_zone)->pool);
        // elem = merge_chuncks(elem->prev, elem, (**m_zone)->pool);
    }
    if (!elem->prev && !elem->next)
    {
        if (!destroy_memzone(**m_zone, chunck_size * MAX_ALLOC))
            return (FALSE); // check
        **m_zone = NULL;
    }
    return (TRUE);
}

static t_meta   *search_meta(void *ptr, t_meta *meta)
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

t_meta          *ptr_in_zones(void *ptr, t_memzone ***m_zone, \
    size_t *chunck_size)
{
    void        *start_addr;
    short       type;
    t_memzone   *tmp_zone;
    
    type = TINY;
    while (type < LARGE)
    {
        *m_zone = (type == TINY) ? &g_memory.tiny : &g_memory.small;
        *chunck_size = (type == TINY) ? TINY_CHUNCK_SIZE : SMALL_CHUNCK_SIZE;
        tmp_zone = **m_zone;
        while (tmp_zone)
        {
            start_addr = (void*)(tmp_zone + 1);
            if (ptr >= start_addr && ptr < start_addr + MAX_ALLOC * *chunck_size)
                return (search_meta(ptr, tmp_zone->meta));
            *m_zone = &tmp_zone->next;
            tmp_zone = tmp_zone->next;
        }
        type++;
    }
    *m_zone = NULL;
    return (search_meta(ptr, g_memory.large));
}

void            myfree(void *ptr)
{
    t_meta      *meta;
    t_memzone   **m_zone;
    size_t      chunck_size;

    if (!ptr)
    {
        log_error("error [free] -> null pointer", NULL);
        return ;
    }
    m_zone = &g_memory.tiny;
    if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
        log_error("error [free]: invalid pointer", NULL); // incomplete
    else
    {
        //printf("this is the pointer i have %p and this is large %p\n", meta, g_memory.large);
        //printf("this is the zone i have %p and this is small zone %p\n", m_zone, &g_memory.small);
        if (!m_zone && !destroy_meta(meta, &g_memory.large))
            log_error("error [free] -> destroy_meta()", NULL); // incomplete
        else if (m_zone && !free_elem(&m_zone, chunck_size, meta))
            log_error("error [free] -> merge_chuncks()", NULL); // incomplete
    }
}