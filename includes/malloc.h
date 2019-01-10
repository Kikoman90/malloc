// header 42

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/mman.h>
# include <pthread.h>

# include <string.h> //
# include <stdio.h> //

//utiliser <ncurses.h> (librairie graphique sur le terminal)

# define TINY_CHUNCK_SIZE 128
# define SMALL_CHUNCK_SIZE 4096
# define LARGE_CHUNCK_SIZE UINT_MAX // 2^32 octets

# define MAX_ALLOC 128 //nb d'allocations max par zone

// getrlimit()
// 9GB
// if (getrlimit(RLIMIT_AS) < LARGE_CHUNCK_SIZE) USE RLIMIT !!!
# define MAX_SIZE LARGE_CHUNCK_SIZE

# define MALLOC_DEBUG 1

typedef enum            e_zone
{
    VOID = 0,
    TINY,
    SMALL,
    LARGE
}                       t_zone;

typedef struct			s_meta
{
    void                *addr;
    size_t              size;
    char                used;
    struct s_meta       *prev;
    struct s_meta       *next;
}						t_meta;

typedef struct          s_metapool
{
    t_meta              *pool;
    size_t              size;
    struct s_metapool   *prev;
    struct s_metapool   *next;
}                       t_metapool;

typedef struct          s_memzone
{
    t_metapool          *pool;
    t_meta              *meta;
    struct s_memzone    *prev;
    struct s_memzone    *next;
}                       t_memzone;
//t_meta                *alloc;
//t_meta                *free;
// methode double

typedef struct			s_mem
{
    t_memzone			*tiny;
    t_memzone			*small;
    t_meta              *large;
}						t_mem;

t_mem					g_memory;
pthread_mutex_t			g_mutex;

void					myfree(void *ptr);
void					*mymalloc(size_t size);
void					*myrealloc(void *ptr, size_t size);
void					show_alloc_mem();

int                     hexadiff(char *addr1, char *addr2); //

#endif
