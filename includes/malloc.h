#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/time.h> 
# include <sys/resource.h>
# include <pthread.h>
# include <stdio.h> //

//utiliser <ncurses.h> (librairie graphique sur le terminal)

# define TINY_SIZE 128 // 32
# define SMALL_SIZE 4096 // 512
# define LARGE_SIZE UINT_32 // /4

# define MAX_ALLOC 128 //allocation de memoire maximum

# define INIT 0 //preinitialisation 0: false 1 : true

typedef struct			s_memchunck // liste chainee block memoire
{
    void				*addr; //adresse memoire
    void				*data; //donnee
    struct s_memchunck	*prev; //maillon precedent
    struct s_memchunck	*next; //maillon suivant
}						t_memchunck;

typedef struct			s_mem //structure memoire
{
    t_memchunck			*tiny;
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
