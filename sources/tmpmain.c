#include "malloc.h"

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
    i = 0;
    while (i < 10)
    {
        printf("ptr[%d] = %d\n", i, ptr[i]);
        i++;
    }
    return (0);
}