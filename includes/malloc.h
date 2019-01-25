// header 42

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include <sys/mman.h>
# include <pthread.h>

# include <errno.h>
# include <string.h>

# include <stdio.h> //

# define TINY_CHUNCK_SIZE 128
# define SMALL_CHUNCK_SIZE 4096

# define MAX_SIZE UINT_MAX

# define ALIGNMENT 16

# define POOL_SIZE 4096

# define MAX_ALLOC 32

# define TRUE 1
# define FALSE 0

# define TINY 1
# define SMALL 2
# define LARGE 3

# ifndef MALLOC_DEBUG
#  define MALLOC_DEBUG 0
# endif

typedef uint64_t		t_ull_64;

typedef struct			s_meta
{
	void				*addr;
	size_t				size;
	char				used;
	struct s_meta		*prev;
	struct s_meta		*next;
}						t_meta;

typedef struct			s_metapool
{
	t_meta				*pool;
	size_t				size;
	struct s_metapool	*prev;
	struct s_metapool	*next;
}						t_metapool;

typedef struct			s_memzone
{
	t_metapool			*pool;
	t_meta				*meta;
	struct s_memzone	*prev;
	struct s_memzone	*next;
}						t_memzone;

typedef struct			s_mem
{
	t_memzone			*tiny;
	t_memzone			*small;
	t_meta				*large;
}						t_mem;

t_mem					g_memory;
pthread_mutex_t			g_mutex;

/*
** show_alloc_mem.c     => 2 functions
*/
void					show_alloc_mem();

/*
** show_alloc_mem_ex.c  => 4 functions
*/
void					show_alloc_mem_ex(void);

/*
** malloc.c             => 4 functions
*/
void					*malloc_unsafe(size_t size);
void					*malloc(size_t size);

/*
** free.c               => 4 functions
*/
int						free_elem(t_memzone ***m_zone, size_t chunck_size, \
							t_meta *elem);
t_meta					*ptr_in_zones(void *ptr, t_memzone ***m_zone, \
							size_t *chunck_size);
void					free(void *ptr);

/*
** realloc.c            => 5 functions
*/
void					*realloc(void *ptr, size_t size);

/*
** calloc.c				=> 3 functions
*/
void					*ft_memcpy(void *dest, void *src, size_t len);
void					*ft_memset(void *b, int c, size_t len);
void					*calloc(size_t nmemb, size_t size);

/*
** heap.c				=> 4 functions
*/
t_metapool				*create_metapool(size_t nb_meta);
t_memzone				*create_memzone(size_t chunck_size);
int						destroy_metapools(t_metapool *pool);
int						destroy_memzone(t_memzone *zone, t_memzone **head, \
							size_t size);

/*
** meta.c               => 4 functions
*/
int						destroy_meta(t_meta *meta, t_meta **head);
t_meta					*remove_meta(t_meta *meta, t_metapool *pool);
t_meta					*metadip(t_metapool *metapool, void *addr, size_t size);
t_meta					*insert_meta(t_metapool *pool, t_meta *meta, \
							void *addr, size_t size);

/*
** hexa_diff.c          => 1 functions
*/
int						hexadiff(void *addr1, void *addr2);

/*
** display.c			=> 2 functions
*/
void					display_meta(t_meta *meta, size_t *nb_bytes, \
							int display_mem);
void					print_tab(char *tab[], size_t nb_string, \
							t_ull_64 print_octets);

/*
** log.c                => 2 functions
*/
int						log_error(const char *str1, const char *str2);
void					*log_error_null(const char *str1, const char *str2);

/*
** print_tools.c		=> 5 functions
*/
int						ft_strlen(char const *str);
void					ft_putchar(char c);
void					ft_putchar_fd(short fd, char c);
void					ft_putstr(char const *s);
void					ft_putstr_fd(short fd, char const *str);

/*
** tools.c				=> 5 functions
*/
void					ft_putendl(char const *s);
void					ft_print_unsigned_long_long(t_ull_64 n);
void					ft_print_uc_hex(unsigned char n);
size_t					align(size_t size);
size_t					align_to_page(size_t size);

/*
** show_alloc_tools.c   => 4 functions
*/
void					print_addr(void *addr, short ret_line);
void					print_ascii(unsigned char *buff, size_t *pos);
void					tab_ascii(unsigned char *s, size_t *pos, \
							size_t l_oct, unsigned char c);
void					print_tab(char *tab[], size_t nb_string, \
							t_ull_64 print_octets);

/*
** ft_itoa_addr_hex.c   => 2 functions
*/
char					*ft_itoa_addr(t_ull_64 value, char *s, \
							t_ull_64 size, short print_ox);

#endif
