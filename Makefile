NAME = cube3D
SRCS = $(shell find "." -name "*.c")
OBJS = ${SRCS:.c=.o}
CC = gcc
RM = rm -f
CFLAGS		= -Wall -Wextra -Werror
AR			= ar csr

%.o: %.c
	$(CC) $(CFLAGS) -Imlx -c $< -o $@

$(NAME): $(OBJ)
	$(CC) -lmlx -framework OpenGL -framework AppKit cub.c && ./a.out

all : ${NAME}

clean :
	${RM} ${OBJS} ${BONUS_OBJS}

fclean : clean
	${RM} ${NAME}

re : fclean all

.PHONY: all clean fclean bonus re .c.o