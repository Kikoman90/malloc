 // 42 header

#include "malloc.h"

void    *realloc_large(t_meta *meta, size_t size)
{
    long    diff;

    diff = size - meta->size;
    if (!diff)
        return (meta->addr);
    if (!destroy_meta(meta, &g_memory.large))
        return (log_error_null("error [realloc]: ...", NULL)); // incomplete
    return (malloc_large(size));
}

void    *realloc_tiny_or_small(t_memzone ***m_zone, size_t chunck_size, \
    t_meta *meta, size_t size)
{
    long    diff;
    t_meta  *insert;

    diff = size - meta->size;
    if (!diff)
        return (meta->addr);
    if (diff < 0 || \
        (meta->next && !meta->next->used && (long)meta->next->size >= size))
    {
        meta->size += diff;
        if (meta->next && !meta->next->used && (long)meta->next->size >= size)
        {
            meta->next->addr = (char*)meta->next->addr + diff;
            if (!(meta->next->size -= diff))
                remove_meta(meta->next, (**m_zone)->pool);
            return (meta->addr);
        }
        if (!(insert = insert_meta((**m_zone)->pool, meta, \
            (void*)((char*)meta->addr + meta->size), (size_t)(-diff))))
            return (log_error_null("nope", NULL)); // incomplete
        insert->used = FALSE;
        return (meta->addr);
    }
    if (!free_elem(m_zone, chunck_size, meta))
        return (log_error_null("nope", NULL)); // incomplete
    return (malloc_tiny_or_small(size, chunck_size, *m_zone));
}

void    *myrealloc(void *ptr, size_t size)
{
    t_meta      *meta;
    t_memzone   **m_zone;
    size_t      chunck_size;
    size_t      diff;

    m_zone = &g_memory.tiny;
    if (!ptr)
        return (mymalloc(size));
    if (size > MAX_SIZE)
        return log_error_null("error [realloc]: size is invalid", NULL);
    if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
    {
        if (*m_zone)
            return (realloc_tiny_or_small(&m_zone, chunck_size, meta, size));
        return (realloc_large(meta, size));
    }
    return (log_error_null("error [realloc]: ptr is invalid", NULL)); // check
}