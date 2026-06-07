NAME = codexion
CC = cc 
# CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread
CFLAGS = -pthread -fsanitize=thread
LEAKS =  valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./executable exampleParam1


# Source files
SRCS = codexion.c \
	   parsing.c \
       ft_split.c \
       libft.c \
       helper.c \
	   rules.c \
	   threads.c \
	   simulation.c \
	   time.c \
	   dongles.c


# Object files
OBJS = $(SRCS:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


%.o: %.c codexion.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓ Compiled: $<$(RESET)"

clean:
	@rm -f $(OBJS) $(OBJS_BNS)
	@echo "$(RED)✗ Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME) 

re: fclean all

.PHONY: all bonus clean fclean re