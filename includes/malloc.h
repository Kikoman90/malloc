// header 42

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/mman.h>
// # include <sys/time.h> 
// # include <sys/resource.h>
# include <pthread.h>

# include <stdio.h> //

//utiliser <ncurses.h> (librairie graphique sur le terminal)

# define TINY_MAX_SIZE 128
# define SMALL_MAX_SIZE 4096
# define LARGE_MAX_SIZE UINT_MAX // 2^32 octets

# define MAX_ALLOC 128 //nb d'allocations max par zone

# define TINY_INIT 0
# define SMALL_INIT 0
# define LARGE_INIT 0

// # define TRUE 1
// # define FALSE 0

typedef struct			s_memchunck
{
    //char              used;
    size_t              size;
    struct s_memchunck	*prev;
    struct s_memchunck	*next;
}						t_memchunck;

typedef struct          s_memzone
{
    t_memchunck         *alloc;
    t_memchunck         *free;
    struct s_memzone    *prev;
    struct s_memzone    *next;
}                       t_memzone;

typedef struct			s_mem
{
    t_memzone			*tiny;
    t_memchunck			*small;
    t_memchunck			*large;
}						t_mem;

t_mem					g_memory;
pthread_mutex_t			g_mutex;

void					myfree(void *ptr);
void					*mymalloc(size_t size);
void					*myrealloc(void *ptr, size_t size);
void					show_alloc_mem();

#endif
