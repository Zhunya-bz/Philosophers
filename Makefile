SRCS = main.c ft_atoi.c time.c philo_doing.c philo_die.c create_thread.c philo_print.c

OBJS = ${SRCS:.c=.o}

NAME = philo

RM = rm -f

CC = gcc

CFLAGS = -Wall -Wextra -Werror

HEADER = philo.h

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

${NAME}:	${OBJS} ${HEADER}
			${CC} ${CFLAGS} ${OBJS} -o ${NAME}


all:    ${NAME}

clean:
		${RM} ${OBJS} ${OBJS_BONUS}

fclean: clean
		${RM} ${NAME}

re:     fclean all

.PHONY: fclean clean all re bonus