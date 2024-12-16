/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:11:00 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 15:41:38 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	ft_mutex_print(philo, "has taken a fork");
	ft_wait(philo->settings, philo->settings->time_to_eat);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_lock(&philo->settings->critical_region);
	philo->settings->dead_philo = 0;
	pthread_mutex_unlock(&philo->settings->critical_region);
	return (NULL);
}

static void	philo_loop(t_philo *philo)
{
	while (true)
	{
		if (simu_done(philo->settings))
			return ;
		think(philo);
		if (simu_done(philo->settings))
			return ;
		pickup_forks(philo);
		if (simu_done(philo->settings))
		{
			pthread_mutex_unlock(philo->left);
			pthread_mutex_unlock(philo->right);
			return ;
		}
		eat(philo->settings, philo);
		if (simu_done(philo->settings))
			return ;
		philo_sleep(philo->settings, philo);
		if (simu_done(philo->settings))
			return ;
		if (philo->id % 2 == 0)
			usleep(750);
	}
	return ;
}

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(&philo->settings->critical_region);
	philo->start_time = get_cur_time_ms();
	pthread_mutex_unlock(&philo->settings->critical_region);
	if (philo->settings->n_philos == 1)
		return (single_philo(philo));
	if (philo->id % 2 == 0)
		usleep(750);
	philo_loop(philo);
	return (NULL);
}

static pthread_t	*start_philos(t_philo *philos)
{
	int			i;
	pthread_t	*threads;

	threads = malloc(sizeof(pthread_t) * philos->settings->n_philos);
	if (!threads)
		return (NULL);
	i = 0;
	while (i < philos->settings->n_philos)
	{
		if (pthread_create(&threads[i], NULL, &philo_routine, &philos[i]) != 0)
		{
			printf("fail to create thread\n");
			philos->settings->simu_done = true;
			join_threads(threads, i - 1);
			return (NULL);
		}
		i++;
	}
	return (threads);
}

void	simulate(t_philo *philos)
{
	pthread_t	*threads;
	pthread_t	monitor_thd;

	if (pthread_create(&monitor_thd, NULL, &monitor_routine, philos) != 0)
	{
		printf("fail to create thread\n");
		return ;
	}
	threads = start_philos(philos);
	if (!threads)
	{
		pthread_join(monitor_thd, NULL);
		return ;
	}
	pthread_join(monitor_thd, NULL);
	join_threads(threads, philos->settings->n_philos - 1);
	return ;
}
