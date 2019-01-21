#include "malloc.h"

void defragmentation(t_memzone *m_zone)
{
    t_memzone *tmp;
    t_meta *swp_elem;
    t_meta *swp_zone;

    tmp = m_zone;
    while (tmp)
    {
        swp_zone = tmp->meta;
        while(swp_zone)
        {
            swp_zone = swp_zone->next;
        }
        tmp = tmp->next;
    }
}

//           []
/// [] [] [] [] [] [] [] 