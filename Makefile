NAME = codexion
CC = cc 
CFLAGS = -Wall -Wextra -Werror
# LEAKS = -fsanitize=address -g

# Source files
SRCS = codexion.c \
	   parsing.c \
       ft_split.c \
       libft.c \
       helper.c \
	   rules.c


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