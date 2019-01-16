// header 42

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/mman.h>
# include <pthread.h>

# include <errno.h>

# include <string.h> //
# include <stdio.h> //

//utiliser <ncurses.h> (librairie graphique sur le terminal)

# define TINY_CHUNCK_SIZE 128
# define SMALL_CHUNCK_SIZE 4096

# define MAX_SIZE UINT_MAX

# define POOL_SIZE 4096

# define MAX_ALLOC 128

# define TRUE 1
# define FALSE 0

# define TINY 1
# define SMALL 2
# define LARGE 3

// getrlimit()
// 9GB
// if (getrlimit(RLIMIT_AS) < MAX_SIZE) USE RLIMIT !!!

# define MALLOC_DEBUG 1

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

typedef struct			s_mem
{
    t_memzone			*tiny;
    t_memzone			*small;
    t_meta              *large;
}						t_mem;

t_mem					g_memory;
pthread_mutex_t			g_mutex;

void                    show_alloc_mem();
void                    show_alloc_mem_ex(void);

/*
** malloc.c             => 4 functions
*/
void                    *malloc_large(size_t size);
void                    *mymalloc(size_t size);

/*
** free.c               => 4 functions
*/
int                     free_elem(t_memzone ***m_zone, size_t chunck_size, \
                            t_meta *elem);
t_meta                  *ptr_in_zones(void *ptr, t_memzone ***m_zone, \
                            size_t *chunck_size);
void                    myfree(void *ptr);

/*
** realloc.c            => ? functions
*/
void					*myrealloc(void *ptr, size_t size);

/*
** heap.c               => 2 functions
*/
t_metapool              *create_metapool(size_t nb_meta);
t_memzone               *create_memzone(size_t chunck_size);
int                     destroy_metapools(t_metapool *pool);
int                     destroy_memzone(t_memzone *zone, size_t size);

/*
** meta.c               => 4 functions
*/
int                     destroy_meta(t_meta *meta, t_meta **head);
t_meta                  *remove_meta(t_meta *meta, t_metapool *pool);
t_meta                  *metadip(t_metapool *metapool, void *addr, size_t size);
t_meta                  *insert_meta(t_metapool *pool, t_meta *meta, \
                            void *addr, size_t size);

/*
** hexa_diff.c          => ? functions
*/
void	                ft_putchar(char c);
int                     hexadiff(void *addr1, void *addr2);
void                    display_meta(t_meta *meta, size_t *nb_bytes, int display_mem);
void                    print_tab(char *tab[], size_t nb_string, long long print_octets);
void                    ft_putendl(char const *s);
void                    ft_putstr(char const *s);
void                    ft_putchar(char c);
int                     ft_strlen(char const *str);

/*
** log.c                => 2 functions
*/
int                     log_error(const char *str1, const char *str2);
void                    *log_error_null(const char *str1, const char *str2);

#endif
