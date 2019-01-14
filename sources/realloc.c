 // 42 header

#include "malloc.h"



void    *myrealloc(void *ptr, size_t size)
{
    t_meta      *meta;
    t_memzone   **m_zone;
    size_t      chunck_size;

    m_zone = &g_memory.tiny;
    if (!ptr)
        return (mymalloc(size));
    if (size > MAX_SIZE)
    {
        // ...
        return (NULL);
    }
    if (!ptr_in_zones(ptr, &m_zone, &chunck_size))

    return (NULL); // delet this
}