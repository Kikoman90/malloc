 // 42 header

#include "malloc.h"

void    *ft_memcpy(void *dest, void *src, size_t count)
{
	size_t i;
	
	i = 0;
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
    if (!(size - meta->size)) //si la taille indiquee est la taille d'origine, pas de changement
        return (meta->addr);
	// le realloc large va desallouer la zone pointee apres avoir mempcy son contenu dans la nouvelle zone allouee
    if (size)
    {
        if (!(ptr = ft_memcpy(malloc(size), meta->addr, \
            (meta->size < size) ? meta->size : size)))
            return (log_error_null("error [realloc]: malloc has failed", NULL));
    }
    if (!destroy_meta(meta, &g_memory.large))
        return (log_error_null("error [realloc]: destroy_meta has failed", NULL));
    return (ptr);
}

// norminette...
void    *realloc_tiny_or_small(t_memzone ***m_zone, size_t chunck_size, \
    t_meta *meta, size_t size)
{
    long    diff; // difference entre la taille demandee (size) et la taille d'origine (taille allouee a l'adresse pointee: meta->size)
    void    *ptr;
    t_meta  *insert;

    ptr = NULL;
    if (!(diff = size - meta->size)) //si la taille indiquee est la taille d'origine, pas de changement
        return (meta->addr);
	// CAS 1
	// if (taille == 0 || taille hors des bornes de ce type de zone || aucun moyen d'etendre notre maillon*)
	// * maillon inexistant ou non-libre ou ayant une taille insufissante
	// => on doit malloc ailleurs, et marquer l'espace precedent comme libre apres avoir memcpy son contenu
    if (!size || (size <= TINY_CHUNCK_SIZE && chunck_size == SMALL_CHUNCK_SIZE)\
        || size > chunck_size || (diff > 0 && (!meta->next || meta->next->used\
        || meta->next->size < (size_t)diff)))
    {
        if (size && !(ptr = ft_memcpy(malloc(size), meta->addr, \
            (meta->size < size) ? meta->size : size)))
            return (log_error_null("error [realloc]: malloc has failed", NULL));
        if (!free_elem(m_zone, chunck_size, meta))
            return (log_error_null("error [realloc]: free has failed", NULL));
        return (ptr);
    }
	// CAS 2 : soit la diff est negative, soit elle est positive.
	// SI CAS 1 est faux et que la diff est positive, alors il existe forcement un maillon libre avec une taille suffisante
    meta->size += diff;
    if (meta->next && !meta->next->used && (long)meta->next->size >= diff)
    {
        meta->next->addr = (char*)meta->next->addr + diff;
        if (!(meta->next->size -= diff))
            remove_meta(meta->next, (**m_zone)->pool);
        return (meta->addr);
    }
	// sinon, la difference est negative, donc la taille demandee est inferieure a la taille d'origine.
	// on doit quand meme inserer un maillon libre, d'une taille egale a la difference
    if (!(insert = insert_meta((**m_zone)->pool, meta, \
        (void*)((char*)meta->addr + meta->size), (size_t)(-diff)))) // 
        return (log_error_null("error [realloc]: insert_meta has failed", NULL));
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
        return (log_error_null("error [realloc]: ptr is invalid", NULL));
    if (m_zone)
        return (realloc_tiny_or_small(&m_zone, chunck_size, meta, size));
    return (realloc_large(meta, size));
}