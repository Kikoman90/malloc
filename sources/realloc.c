 // 42 header

#include "malloc.h"

// if ptr is null -> malloc(size) => invalid size handled there
// if ptr is invalid -> do nothing
// if ptr is valid and size is 0 -> free(ptr) ?????
// if ptr is valid and size is > UINT_32 -> ?????

void    *realloc_large(t_meta *meta, size_t size)
{
    size_t  diff;

    diff = meta->size - size;
    if (!diff)
        return (meta->addr);
    
}

void    *realloc_tiny_or_small(t_meta *meta)
{

}

void    *myrealloc(void *ptr, size_t size)
{
    t_meta      *meta;
    t_meta      *insert;
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
            return (realloc_tiny_or_small(meta, size));
        return (realloc_large(meta, size));
    }
    /*
        diff = meta->size - size;
        // case size < meta->size
        if (meta->next && !meta->next->used)
            ;//extend meta
        else
        {
            meta->size = size;
            if (size < meta->size && *m_zone)
            {
                if (!(insert = insert_meta((*m_zone)->pool, meta, \
                    meta->addr + meta->size, diff)))
                    return (log_error_null("error [realloc]...", NULL)); // incomplete
                insert->used = 0;
            }
            else if (size > meta->size)
            {
                //
            }
        }
    */
    return (log_error_null("error [realloc]: ptr is invalid", NULL)); // check
}