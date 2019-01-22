 // 42 header

#include "malloc.h"

void    *ft_memcpy(void *dest, void *src, size_t count)
{
    size_t i = 0;

    while (i < count)
    {
        ((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
        i++;
    }
    return (dest);
}

void    *realloc_large(t_meta *meta, size_t size)
{
    void    *ptr;

    ptr = NULL;
    if (!(size - meta->size))
        return (meta->addr);
    if (size)
    {
        if (!(ptr = ft_memcpy(malloc(size), meta->addr, \
            (meta->size < size) ? meta->size : size)))
            return (log_error_null("error [realloc]: ...", NULL)); // incomplete
    }
    if (!destroy_meta(meta, &g_memory.large))
        return (log_error_null("error [realloc]: ...", NULL)); // incomplete
    return (ptr);
}

// norminette...
void    *realloc_tiny_or_small(t_memzone ***m_zone, size_t chunck_size, \
    t_meta *meta, size_t size)
{
    long    diff;
    void    *ptr;
    t_meta  *insert;

    ptr = NULL;
    if (!(diff = size - meta->size))
        return (meta->addr);
    if (!size || (size <= TINY_CHUNCK_SIZE && chunck_size == SMALL_CHUNCK_SIZE)\
        || size >= chunck_size || (diff > 0 && (!meta->next || meta->next->used\
        || meta->next->size < (size_t)diff)))
    {
        if (size && !(ptr = ft_memcpy(malloc(size), meta->addr, \
            (meta->size < size) ? meta->size : size)))
            return (log_error_null("error [realloc]: ...", NULL)); // incomplete
        if (!free_elem(m_zone, chunck_size, meta))
            return (log_error_null("error [realloc]:...", NULL)); // incomplete
        return (ptr);
    }
    meta->size += diff;
    if (meta->next && !meta->next->used && (long)meta->next->size >= diff)
    {
        meta->next->addr = (char*)meta->next->addr + diff;
        if (!(meta->next->size -= diff))
            remove_meta(meta->next, (**m_zone)->pool);
        return (meta->addr);
    }
    if (!(insert = insert_meta((**m_zone)->pool, meta, \
        (void*)((char*)meta->addr + meta->size), (size_t)(-diff)))) // 
        return (log_error_null("nope", NULL)); // incomplete
    insert->used = FALSE;
    return (meta->addr);
}

void    *realloc(void *ptr, size_t size)
{
    t_meta      *meta;
    t_memzone   **m_zone;
    size_t      chunck_size;

    if (!ptr)
        return (malloc(size));
    if (size > MAX_SIZE)
        return log_error_null("error [realloc]: size is invalid", NULL);
    if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
        return (log_error_null("error [realloc]: ptr is invalid", NULL)); // check
    if (m_zone)
        return (realloc_tiny_or_small(&m_zone, chunck_size, meta, size));
    return (realloc_large(meta, size));
}