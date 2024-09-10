/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/10 16:31:46 by leon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h>

long long	get_cur_time_ms()
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_mutex_print(long long time, int id, char *msg, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	printf("%lli %i %s\n", time, id, msg);
	pthread_mutex_unlock(mutex);
}

void	kill_philo(int id, t_settings *settings)
{
	pthread_mutex_lock(&settings->dead_philo_lock);
	settings->dead_philo = id;
	pthread_mutex_unlock(&settings->dead_philo_lock);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, id,
				"died", &settings->print_lock);
}

bool	all_alive(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(&philo->settings->dead_philo_lock);
	if (philo->settings->dead_philo == -1)
	{
		pthread_mutex_unlock(&philo->settings->dead_philo_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->settings->dead_philo_lock);
	return (false);
}

void	think(t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - philo->settings->start_time, philo->id,
				"is thinking", &philo->settings->print_lock);
	usleep(100);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	ft_mutex_print(get_cur_time_ms() - philo->settings->start_time, philo->id,
				"has taken a fork", &philo->settings->print_lock);
	pthread_mutex_lock(philo->right);
	ft_mutex_print(get_cur_time_ms() - philo->settings->start_time, philo->id,
				"has taken a fork", &philo->settings->print_lock);
	ft_mutex_print(get_cur_time_ms() - philo->settings->start_time, philo->id,
				"is eating", &philo->settings->print_lock);
	philo->started_eating = get_cur_time_ms();
	usleep(philo->settings->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	philo->times_eaten++;
}

void	philo_sleep(t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - philo->settings->start_time, philo->id,
				"is sleeping", &philo->settings->print_lock);
	usleep(philo->settings->time_to_sleep * 1000);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (all_alive(arg))
	{
		think(philo);
		if (!all_alive(arg))
			break ;
		eat(philo);
		if (!all_alive(arg))
			break ;
		philo_sleep(philo);
		if (philo->started_eating == -1)
		{
			if ((get_cur_time_ms() - philo->start_time) > philo->settings->time_to_die)
				kill_philo(philo->id, philo->settings);
		}
		else
		{
			if ((get_cur_time_ms() - philo->started_eating) > philo->settings->time_to_die)
				kill_philo(philo->id, philo->settings);
		}
	}
	return (NULL);
}

void	simulate(t_settings *settings, t_philo *philos)
{
	int			i;
	pthread_t	*threads;
//	pthread_t	monitor;

	threads = malloc(sizeof(pthread_t) * settings->n_philos);
	if (!threads)
	{
		// fail malloc
	}
//	pthread_create(&monitor, NULL, &monitor_routine, NULL);
	i = 0;
	while (i < settings->n_philos)
	{
		pthread_create(&threads[i], NULL, &philo_routine, &philos[i]);
		i++;
	}
	i = 0;
	while (i < settings->n_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_settings settings;

	if (argc < 5 || argc > 6)
	{
		printf("Error: need 4 or 5 arguments\n");
		return (1);
	}
	settings.n_philos = ft_atoi(argv[1]);
	settings.time_to_die = ft_atoi(argv[2]);
	settings.time_to_eat = ft_atoi(argv[3]);
	settings.time_to_sleep = ft_atoi(argv[4]);
	settings.dead_philo = -1;
	settings.start_time = get_cur_time_ms();
	pthread_mutex_init(&settings.dead_philo_lock, NULL);
	pthread_mutex_init(&settings.print_lock, NULL);
	if (argc == 6)
		settings.n_meals = ft_atoi(argv[5]);
	else
		settings.n_meals = -1;
	printf("%i, %i, %i, %i, %i\n", settings.n_philos, settings.time_to_die, 
		settings.time_to_eat, settings.time_to_sleep, settings.n_meals);
	if (init(&settings))
		return (1);
	return (0);
}
