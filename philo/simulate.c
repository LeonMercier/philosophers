/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:11:00 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/10 17:29:21 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
bool	all_alive(void *arg)
{
	t_settings	*settings;

	settings = (t_settings *) arg;
	pthread_mutex_lock(&settings->critical_region);
	if (settings->dead_philo == -1)
	{
		pthread_mutex_unlock(&settings->critical_region);
		return (true);
	}
	pthread_mutex_unlock(&settings->critical_region);
	return (false);
}
*/
void	think(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo,
		"is thinking");
}

void	eat(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo,
				"is eating");
//	pthread_mutex_lock(&settings->critical_region);
	philo->started_eating = get_cur_time_ms();
//	pthread_mutex_unlock(&settings->critical_region);
	usleep(settings->time_to_eat * 1000);
//	pthread_mutex_lock(&settings->critical_region);
	philo->times_eaten++;
//	pthread_mutex_unlock(&settings->critical_region);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

void	philo_sleep(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo,
		"is sleeping");
	usleep(settings->time_to_sleep * 1000);
}

// philo will be blocked in this function until they can pick up both forks 
// conceptually they will be thinking
void	pickup_forks(t_settings *settings, t_philo *philo)
{
	if (settings->n_philos == 1)
	{
		pthread_mutex_lock(philo->left);
		ft_mutex_print(get_cur_time_ms() - settings->start_time, philo,
			"has taken a fork");
		usleep(settings->time_to_die * 1000);
		pthread_mutex_unlock(philo->left);
		kill_philo(philo, settings);
		return ;
	}
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->left);
	else
		pthread_mutex_lock(philo->right);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo,
		"has taken a fork");
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->right);
	else
		pthread_mutex_lock(philo->left);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo,
		"has taken a fork");
}

void	*philo_routine(void *arg)
{
	// TODO: when exiting thread, release all locks
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (true)
	{
		think(philo->settings, philo);
		pickup_forks(philo->settings, philo);
		/*
		if (!all_alive(philo->settings))
		{
			pthread_mutex_unlock(philo->left);
			pthread_mutex_unlock(philo->right);
			return (NULL);
		}
		*/
		eat(philo->settings, philo);
		philo_sleep(philo->settings, philo);
//		pthread_mutex_lock(&philo->settings->critical_region);
		if (philo->settings->simu_done)
		{
//			pthread_mutex_unlock(&philo->settings->critical_region);
			return (NULL);
		}
//		pthread_mutex_unlock(&philo->settings->critical_region);
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
	philos->settings->start_time = get_cur_time_ms();
	i = 0;
	while (i < philos->settings->n_philos)
	{
		if (pthread_create(&threads[i], NULL, &philo_routine, &philos[i]) != 0)
		{
			printf("fail to create thread\n");
			// join threads created so far?
			free(threads);
			return ;
		}
		i++;
	}
	pthread_join(monitor_thd, NULL);
	i = 0;
	while (i < philos->settings->n_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
	return ;
}
