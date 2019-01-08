// header 42

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/mman.h>
# include <pthread.h>

# include <stdio.h> //

//utiliser <ncurses.h> (librairie graphique sur le terminal)

# define TINY_CHUNCK_SIZE 128
# define SMALL_CHUNCK_SIZE 4080
# define LARGE_CHUNCK_SIZE UINT_MAX // 2^32 octets

# define MAX_ALLOC 128 //nb d'allocations max par zone

# define SIZEOF_META 32

# define MALLOC_DEBUG 1

// create pools of size = PAGESIZE (4096)
// then only 

typedef struct			s_meta
{
    void                *addr;
    size_t              size;
    struct s_meta       *prev;
    struct s_meta       *next;
}						t_meta;

typedef struct          s_memzone
{
    t_meta              *alloc;
    t_meta              *free;
    struct s_memzone    *prev;
    struct s_memzone    *next;
}                       t_memzone;

typedef struct			s_mem
{
    t_memzone			*tiny;
    t_memzone			*small;
    t_memzone			*large;
    t_meta              *metapool;
}						t_mem;

t_mem					g_memory;
pthread_mutex_t			g_mutex;

void					myfree(void *ptr);
void					*mymalloc(size_t size);
void					*myrealloc(void *ptr, size_t size);
void					show_alloc_mem();

#endif
