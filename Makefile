CC = cc 
CFLAGS = -Wall -Wextra -Werror 
SRCS = main.c utils.c
OBJCS = $(SRCS:.c=.o)
NAME = philo

all: $(NAME)

$(NAME) : $(OBJCS)
	$(CC) $(CFLAGS) -lpthread -o $@ $^

clean: 
	rm -rf $(OBJCS)

fclean: clean 
	rm -rf $(NAME)

re: fclean all 
	