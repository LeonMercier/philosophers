/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 11:40:55 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/27 10:09:45 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <unistd.h> // usleep()
# include <stdlib.h> //malloc()
# include <stdio.h> // printf()
# include <stdbool.h> // bool type
# include <sys/time.h> // gettimeofday()

typedef struct s_settings
{
	int				n_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_meals;
	int				dead_philo;
	long long		start_time;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	critical_region;
}	t_settings;

typedef struct s_philo
{
	int				id;
	bool			eating;
	long long		start_time;
	long long		started_eating;
	int				times_eaten;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	t_settings		*settings;
}	t_philo;

typedef struct s_two_ptr
{
	t_settings	*settings;
	t_philo		*philos;
}	t_two_ptr;

// init.c
int			init(t_settings *settings);
// ft_atoi.c
bool		is_valid_number(char *s);
int			ft_atoi(char *s);
// philo.c
void		simulate(t_philo *philos);
long long	get_cur_time_ms(void);
#endif
