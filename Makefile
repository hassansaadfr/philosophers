NAME			=	philo

SRCS			=	philo/main.c \
					philo/philo.c \
					philo/routine.c \
					philo/actions.c \
					philo/init.c \
					philo/parser.c \
					philo/utils.c

OBJS			=	${addprefix srcs/,${SRCS:.c=.o}}

HEAD			=	-I srcs/philo/includes

CC				=	clang

CFLAGS			=	-Wall -Werror -Wextra #-g3 -fsanitize=thread

.c.o			:
					${CC} ${CFLAGS} ${HEAD} -c $< -o ${<:.c=.o}

$(NAME)			:	${OBJS}
					${CC} ${CFLAGS} ${OBJS} -lpthread -o ${NAME}

all				:	${NAME}

clean			:
					@rm -rf ${OBJS}

fclean			:	clean
					@rm -rf ${NAME}

re				:	fclean all

.PHONY			:	all clean fclean re

