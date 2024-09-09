# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/09/09 17:48:38 by leon             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc
CFLAGS	:= -g -Wextra -Wall -Werror -pthread
#CFLAGS	:= 
NAME	:= philo
SRCS	:= philo.c ft_atoi.c init.c
OBJS	:= ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re
