# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imansar <imansar@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/14 15:14:53 by imansar           #+#    #+#              #
#    Updated: 2026/06/21 16:02:54 by imansar          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
CC = cc 
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread

LEAKS =  valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./executable exampleParam1


SRCS = codexion.c \
	   parsing.c \
       ft_split.c \
       libft.c \
       libft2.c \
	   helper.c \
	   errors.c \
	   threads.c \
	   simulation.c \
	   time.c \
	   dongles.c \
	   dongles_helper.c \
	   queue.c 


OBJS = $(SRCS:.c=.o)

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