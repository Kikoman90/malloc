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
    int     *ptr_large2;
    char    *ptr_small;
    size_t  size_ptr[4] = {4, 1, 8, 4};

    ptr1 = (int*)mymalloc(sizeof(int) * 10);
    ptr2 = (char*)mymalloc(sizeof(char) * 10);
    ptr3 = (long*)mymalloc(sizeof(long) * 10);
    ptr4 = (int*)mymalloc(sizeof(int) * 10);
    ptr_small =(char *)mymalloc(sizeof(char) * 3120); 
    //3290 octets
    ptr_large = (int*)mymalloc(sizeof(int) * 4120);
    ptr_large2 = (int*)mymalloc(sizeof(int) * 9000);
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
    show_alloc_mem();
    printf("\033[0m-------------------||---------------------\n");    
    //myfree(ptr4 + 1);
   // display_meta(g_memory.large, size_ptr, 0);
    myfree(ptr_large);
    printf("-------------------||---------------------\n");
    show_alloc_mem();
    printf("\033[0m-------------------||---------------------\n");    
    //myfree(ptr4 + 1);
   // display_meta(g_memory.large, size_ptr, 0);
    ptr_large2 = myrealloc(ptr_large2, 120);
    printf("-------------------||---------------------\n");
    show_alloc_mem();
    printf("-------------------||---------------------\n");
    myfree(ptr_small);
    printf("-------------------||---------------------\n");
    show_alloc_mem();
    printf("-------------------||---------------------\n");
    myfree(ptr2);
    show_alloc_mem();
    printf("-------------------||---------------------\n");
    ptr1 = myrealloc(ptr1, 48);
    ptr_large2 = mymalloc(2);
    show_alloc_mem();
    printf("JAYSON CA A L'AIR DE MARCHER\nESPECE DE VIEILLE MOUETTE DEGARNIE\n");
    //show_alloc_mem_ex();
    /*display_meta(g_memory.tiny->meta, size_ptr, 0);
    printf("-------------------||---------------------\n");
    show_alloc_mem_ex();*/

    return (0);
}