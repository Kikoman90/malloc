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
    int     *ptr_large;
    size_t  size_ptr[4] = {4, 1, 8, 4};

    ptr1 = (int*)mymalloc(sizeof(int) * 10);
    ptr2 = (char*)mymalloc(sizeof(char) * 10);
    ptr3 = (long*)mymalloc(sizeof(long) * 10);
    ptr4 = (int*)mymalloc(sizeof(int) * 10);
    ptr_large = (int*)mymalloc(sizeof(int) * 4120);
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
    display_meta(g_memory.tiny->meta, size_ptr, 0);
    myfree(ptr3);
    display_meta(g_memory.tiny->meta, size_ptr, 0);
    myfree(ptr2);
    display_meta(g_memory.tiny->meta, size_ptr, 0);
    myfree(ptr4);
    //myfree(ptr4 + 1);
    if (g_memory.tiny) {
        display_meta(g_memory.tiny->meta, size_ptr, 0);
    }
    myfree(ptr_large);
    if (g_memory.large)
        ;//display_meta(g_memory.large, 4, 0);
    else
        printf("no memory large\n");
    return (0);
}