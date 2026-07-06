NAME	= pipex
SRCS	= main.c \
		  src/app.c \
		  src/cmd.c \
		  src/cmd_mgr.c \
		  src/error.c \
		  src/parser.c \
		  src/pipe_mgr.c \
		  util/create_cmd_path.c
OBJS = $(SRCS:.c=.o)

FT_PRINTF_DIR = ft_printf
FT_PRINTF_LIB = ftprintf
FT_PRINTF = $(FT_PRINTF_DIR)/lib$(FT_PRINTF_LIB).a

LIB_DIR_FLAGS = -L$(FT_PRINTF_DIR)
LIB_FLAGS = -l$(FT_PRINTF_LIB)

CC = cc
INCLUDES = -Iinclude -Iutil -Ift_printf -Ift_printf/libft
CFLAGS = -Wall -Wextra -Werror -g
HEADERS = 	$(wildcard include/*.h) \
			$(wildcard util/*.h)	\
			$(wildcard ft_printf/*.h)	\
			$(wildcard ft_printf/libft/*.h)	\

all: $(NAME)

$(NAME): $(FT_PRINTF) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_DIR_FLAGS) $(LIB_FLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(FT_PRINTF):
	$(MAKE) -C $(FT_PRINTF_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(FT_PRINTF_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(FT_PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re