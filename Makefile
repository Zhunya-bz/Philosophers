SRCS = main.c ft_atoi.c

OBJS = ${SRCS:.c=.o}

NAME = philo

RM = rm -f

CC = gcc

CFLAGS = -Wall -Wextra -Werror

HEADER = philo.h

%.o: %.c
	${CC} ${CFLAGS} -Imlx -c $< -o $@

${NAME}:	${OBJS} ${HEADER}
			${CC} ${CFLAGS} ${OBJS} -o ${NAME}


all:    ${NAME}

clean:
		${RM} ${OBJS} ${OBJS_BONUS}

fclean: clean
		${RM} ${NAME}

re:     fclean all

.PHONY: fclean clean all re bonus