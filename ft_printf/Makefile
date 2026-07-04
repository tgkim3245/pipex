NAME = libftprintf.a
cc = gcc
CFLAGS = -Wall -Wextra -Werror

LIB_DIR = libft
LIB = $(LIB_DIR)/libft.a

SRCS = 	ft_printf.c \
		print_format_str.c \
		print_format_nbr.c \
		print_format_hex.c \
		util.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIB):
	$(MAKE) -C $(LIB_DIR)

$(NAME): $(LIB) $(OBJS)
	cp $(LIB) $@
	ar rcs $@ $(OBJS)

%.o: %.c ft_printf.h
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

clean:
	$(MAKE) -C $(LIB_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
