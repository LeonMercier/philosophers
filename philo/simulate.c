/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:11:00 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 11:29:25 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_wait(t_settings *settings, long long int to_wait_ms)
{
	long long int	end_time;

	end_time = get_cur_time_ms() + to_wait_ms;
	while (get_cur_time_ms() < end_time)
	{
		usleep(250);
		pthread_mutex_lock(&settings->critical_region);
		if (settings->simu_done)
		{
			pthread_mutex_unlock(&settings->critical_region);
			return (1);
		}
		pthread_mutex_unlock(&settings->critical_region);
	}
	return (0);
}

void	think(t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - philo->start_time, philo,
		"is thinking");
}

void	eat(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - philo->start_time, philo,
				"is eating");
	pthread_mutex_lock(&settings->critical_region);
	philo->started_eating = get_cur_time_ms();
	pthread_mutex_unlock(&settings->critical_region);
	ft_wait(settings, settings->time_to_eat);
	pthread_mutex_lock(&settings->critical_region);
	philo->times_eaten++;
	pthread_mutex_unlock(&settings->critical_region);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

void	philo_sleep(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - philo->start_time, philo,
		"is sleeping");
	ft_wait(settings, settings->time_to_sleep);
}

// philo will be blocked in this function until they can pick up both forks 
// conceptually they will be thinking
void	pickup_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->left);
	else
		pthread_mutex_lock(philo->right);
	ft_mutex_print(get_cur_time_ms() - philo->start_time, philo,
		"has taken a fork");
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->right);
	else
		pthread_mutex_lock(philo->left);
	ft_mutex_print(get_cur_time_ms() - philo->start_time, philo,
		"has taken a fork");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(&philo->settings->critical_region);
	philo->start_time = get_cur_time_ms();
	pthread_mutex_unlock(&philo->settings->critical_region);
	if (philo->settings->n_philos == 1)
	{
		pthread_mutex_lock(philo->left);
		ft_mutex_print(get_cur_time_ms() - philo->start_time, philo,
			"has taken a fork");
		ft_wait(philo->settings, philo->settings->time_to_eat);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_lock(&philo->settings->critical_region);
		philo->settings->dead_philo = 0;
		pthread_mutex_unlock(&philo->settings->critical_region);
		ft_mutex_print(get_cur_time_ms() - philo->start_time, philo, "died");
		return NULL;
	}
	while (true)
	{
		think(philo);
		if (philo->settings->simu_done)
			return (NULL);
		pickup_forks(philo);
		if (philo->settings->simu_done)
			return (NULL);
		eat(philo->settings, philo);
		if (philo->settings->simu_done)
			return (NULL);
		philo_sleep(philo->settings, philo);
		if (philo->settings->simu_done)
			return (NULL);
	}
	return (NULL);
}

void	simulate(t_philo *philos)
{
	int			i;
	pthread_t	*threads;
	pthread_t	monitor_thd;

	threads = malloc(sizeof(pthread_t) * philos->settings->n_philos);
	if (!threads)
	{
		return ;
	}
	if (pthread_create(&monitor_thd, NULL, &monitor_routine, philos) != 0)
	{
		printf("fail to create thread\n");
		free(threads);
		return ;
	}
	i = 0;
	while (i < philos->settings->n_philos)
	{
		if (pthread_create(&threads[i], NULL, &philo_routine, &philos[i]) != 0)
		{
			printf("fail to create thread\n");
			philos->settings->simu_done = true;
			join_threads(threads, i - 1);
			return ;
		}
		i++;
	}
	pthread_join(monitor_thd, NULL);
	join_threads(threads, philos->settings->n_philos - 1);
	return ;
}
