#include "malloc.h"

void __attribute__((destructor)) end() {

    while (getchar() != 'a')
    {
        ;
    }

}

int     main()
{
    int i;
    int *ptr;

    if (!(ptr = (int*)mymalloc(sizeof(int) * 10)))
    {
        printf("malloc failed\n");
        return (0);
    }
    i = 0;
    while (i < 10)
    {
        ptr[i] = 42 + i;
        i++;
    }
    display_meta(g_memory.tiny->meta, 4, 1);
    /*i = 0;
    while (i < 10)
    {
        printf("ptr[%d] = %d\n", i, ptr[i]);
        i++;
    }*/
    myfree(ptr);
    return (0);
}