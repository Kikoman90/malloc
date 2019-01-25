ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc.so

HOSTLIB = libft_malloc_$(HOSTTYPE).so

CC = gcc

FLAGS = -Wall -Wextra -Werror -fPIC -D_THREAD_SAFE -Wno-deprecated-register -MMD

ifeq ($(DEBUG),yes)
	FLAGS += -g -DMALLOC_DEBUG #-fsanitize=address
endif

INC_DIR = includes

SRCS_DIR = sources

OBJS_DIR = tmp

SRCS = 	malloc.c \
		calloc.c \
		realloc.c \
		free.c \
		meta.c \
		heap.c \
		hexa_diff.c \
		ft_itoa_addr_hex.c \
		show_alloc_tools.c \
		show_alloc_mem.c \
		show_alloc_mem_ex.c \
		tools.c \
		print_tools.c \
		display.c \
		log.c \

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

DPDS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.d))

opti:
	@make -j all

debug:
	@make -opti DEBUG=yes

all:
	@make $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) -shared -o $(HOSTLIB) $^
	@ln -fs $(HOSTLIB) $(NAME)
	@echo "\033[35;1m[malloc] compilation success"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(FLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@rm -rf $(NAME)

fclean: clean
	@rm -f $(HOSTLIB)

re:
	@make fclean
	@make

.PHONY: all, clean, fclean, re

-include $(DPDS)
