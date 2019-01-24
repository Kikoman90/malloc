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
    long    *ptr_long[38];
   
    /*
    char    *ptr_small;
    int     *ptr_large;
    int     *ptr_large2;
    ptr_small = (char*)malloc(sizeof(char) * 3120);
    //3290 octets
    ptr_large = (int*)malloc(sizeof(int) * 4120);
    ptr_large2 = (int*)malloc(sizeof(int) * 9000);
    // 19770 octets
    */

    i = 0;
    while (i < 38)
    {
        ptr_long[i] = (long*)malloc(sizeof(long) * 16);
        i++;
    }
    show_alloc_mem_ex(); // barre toi avec ton show_alloc_mem_ex !
    free(ptr_long[37]);
    free(ptr_long[36]);
    free(ptr_long[35]);
    free(ptr_long[34]);
    free(ptr_long[33]);
    free(ptr_long[32]);
    free(ptr_long[31]);
    //free(ptr_long[30]);
    //free(ptr_long[0]);
    //printf("add: %p\n", ptr_long[30]);
    //free(ptr_long[30]);
    printf("freeing the last 8 pointers\n");
    show_alloc_mem_ex();

    //show_alloc_mem_ex();
    return (0);
}
