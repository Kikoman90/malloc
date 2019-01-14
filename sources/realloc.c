 // 42 header

#include "malloc.h"

void    *myrealloc(void *ptr, size_t size)
{
    /*t_meta      *meta;
    t_memzone   **m_zone;
    size_t      chunck_size;
    //void        *ptr;

    m_zone = &g_memory.tiny;
    if (!ptr)
        return (mymalloc(size));
    if (size > MAX_SIZE)
        return log_error_null("error [realloc]: size is invalid", NULL);
    if (!(meta = ptr_in_zones(ptr, &m_zone, &chunck_size)))
    {
        if (meta->next && !meta->next->used && size <= meta->size + meta->next->size)
        {
            if ((meta->next->size -= size - meta->size) == 0)
            {
                remove_elem();
            }
            meta->size = size;
        }
        else
        {
            if (!(ptr = mymalloc(size)))
                return log_error_null("error [realloc]: new malloc has failed", NULL);
            ft_memcpy(ptr, meta->addr, meta->size);
            if (!free_elem(ptr, &m_zone, meta, &chunck_size))
                return log_error_null("error [realloc]: free has failed", NULL);
        }
    }*/
    return (NULL); // delet this
}