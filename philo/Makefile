# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/12/10 16:40:49 by lemercie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc
CFLAGS	:= -g -Wextra -Wall -Werror -pthread
#CFLAGS	:= 
NAME	:= philo
SRCS	:=  common.c ft_atoi.c init.c monitor.c philo.c simulate.c time.c
OBJS	:= ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS) philo.h
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re
