NAME	= pipex

SRCS	= main.c \
		  src/app.c \
		  src/command.c \
		  src/error.c

OBJS = $(SRCS:.c=.o)

CC = cc
INCLUDES = -Iinclude -Iutil
CFLAGS = -Wall -Wextra -Werror -g
HEADERS = $(wildcard include/*.h) $(wildcard util/*.h)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re