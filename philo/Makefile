# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/12/16 16:58:05 by lemercie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc
CFLAGS	:= -Wextra -Wall -Werror -pthread
NAME	:= philo
SRCS	:=  actions.c cleanup.c common.c ft_atoi_safe.c init.c monitor.c \
			philo.c simulate.c time.c
OBJS	:= ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS) philo.h
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re
