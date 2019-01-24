#include "malloc.h"

/*
void __attribute__((destructor)) end() {

    while (getchar() != 'a')
    {
        ;
    }
}
*/

int     main()
{
    int     i;
    int     *ptr1;
    char    *ptr2;
    long    *ptr3;
    int     *ptr4;
    //int     *ptr_large;
    //int     *ptr_large2;
    //char    *ptr_small;

    ptr1 = (int*)malloc(sizeof(int) * 10);
    ptr2 = (char*)malloc(sizeof(char) * 10);
    ptr3 = (long*)malloc(sizeof(long) * 10);
    ptr4 = (int*)malloc(sizeof(int) * 10);
    //ptr_small =(char *)mymalloc(sizeof(char) * 3120); 
    //3290 octets
    //ptr_large = (int*)mymalloc(sizeof(int) * 4120);
   // ptr_large2 = (int*)mymalloc(sizeof(int) * 9000);
    // 19770 octets
    i = 0;
    while (i < 10)
    {
        ptr1[i] = 0 + i;
        ptr2[i] = 'a' + i;
        ptr3[i] = 64 + i;
        ptr4[i] = -10 - i;
        i++;
    }
    show_alloc_mem_ex();
    return (0);
}
