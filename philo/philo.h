/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 11:40:55 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/09 16:56:47 by leon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <unistd.h> // usleep()
# include <stdlib.h> //malloc()
# include <stdio.h> // printf()
# include <stdbool.h> // bool type

typedef struct s_settings
{
	int	n_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	n_meals;
}	t_settings;

typedef struct s_philo
{
	int				id;
	bool			alive;
	int				times_eaten;
	t_settings		*settings;
	pthread_mutex_t	*forks;
}	t_philo;

int	ft_atoi(char *s);
#endif
