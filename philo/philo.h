/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 11:40:55 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 16:58:22 by lemercie         ###   ########.fr       */
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
# include <limits.h> // INT_MAX

typedef struct s_settings
{
	int				n_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_meals;
	int				dead_philo;
	bool			simu_done;
	pthread_mutex_t	critical_region;
}	t_settings;

typedef struct s_philo
{
	int				id;
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

// actions.c
int			ft_wait(t_settings *settings, long long int to_wait_ms);
void		think(t_philo *philo);
void		eat(t_settings *settings, t_philo *philo);
void		philo_sleep(t_settings *settings, t_philo *philo);
void		pickup_forks(t_philo *philo);
// cleanup.c
void		destroy_forks(pthread_mutex_t *forks, int num);
void		join_threads(pthread_t *threads, int index);
// common.c
bool		simu_done(t_settings *settings);
void		ft_mutex_print(t_philo *philo, char *msg);
// init.c
int			init(t_settings *settings);
// ft_atoi_safe.c
int			ft_atoi_safe(const char *s, int *err_atoi);
// monitor.c
void		*monitor_routine(void *arg);
// philo.c
void		simulate(t_philo *philos);
long long	get_cur_time_ms(void);
// simulate.c
void		simulate(t_philo *philos);
// time.c
long long	get_cur_time_ms(void);
#endif
