#include "malloc.h"

void __attribute__((destructor)) end() {

    while (getchar() != 'a')
    {
        ;
    }

}

int     main()
{
    int     i;
    int     *ptr1;
    char    *ptr2;
    long    *ptr3;
    int     *ptr4;
    size_t  size_ptr[4] = {4,1, 8, 4};

    if (!(ptr1 = (int*)mymalloc(sizeof(int) * 10)))
    {
        printf("malloc failed\n");
        return (0);
    }
    ptr2 = (char*)mymalloc(sizeof(char) * 10);
    ptr3 = (long*)mymalloc(sizeof(long) * 10);
    ptr4 = (int*)mymalloc(sizeof(int) * 10);
    i = 0;
    while (i < 10)
    {
        ptr1[i] = 0 + i;
        ptr2[i] = 'a' + i;
        ptr3[i] = 64 + i;
        ptr4[i] = -10 - i;
        i++;
    }
    //display_memory();
    myfree(ptr1);
    display_meta(g_memory.tiny->meta, size_ptr, 1);
    myfree(ptr3);
    display_meta(g_memory.tiny->meta, size_ptr, 1);
    printf("free/alloc/free/alloc/free\n");
    myfree(ptr2);
    myfree(ptr4);
    display_meta(g_memory.tiny->meta, size_ptr, 1);
    //display_meta(g_memory.tiny->meta, size_ptr, 1);
    /*i = 0;
    while (i < 10)
    {
        printf("ptr[%d] = %d\n", i, ptr[i]);
        i++;
    }*/
    //myfree(ptr);
    return (0);
}