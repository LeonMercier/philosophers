/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:36:23 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 16:54:45 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	set_death(t_philo *philos, int i)
{
	pthread_mutex_lock(&philos->settings->critical_region);
	philos->settings->dead_philo = i;
	pthread_mutex_unlock(&philos->settings->critical_region);
	ft_mutex_print(&philos[i], "died");
	return (false);
}

static bool	all_alive(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->settings->n_philos)
	{
		pthread_mutex_lock(&philos->settings->critical_region);
		if (philos[i].started_eating == -1)
		{
			pthread_mutex_unlock(&philos->settings->critical_region);
			if ((get_cur_time_ms() - philos[i].start_time)
				> philos->settings->time_to_die)
				return (set_death(philos, i));
		}
		else
		{
			pthread_mutex_unlock(&philos->settings->critical_region);
			if ((get_cur_time_ms() - philos[i].started_eating)
				> philos->settings->time_to_die)
				return (set_death(philos, i));
		}
		i++;
	}
	return (true);
}

static bool	all_eaten(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->settings->n_philos)
	{
		pthread_mutex_lock(&philos->settings->critical_region);
		if (philos[i].times_eaten < philos->settings->n_meals)
		{
			pthread_mutex_unlock(&philos->settings->critical_region);
			return (false);
		}
		pthread_mutex_unlock(&philos->settings->critical_region);
		i++;
	}
	return (true);
}

// TODO: remove sleep and start monitor after philos instead?
void	*monitor_routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *) arg;
	usleep(1000);
	while (true)
	{
		pthread_mutex_lock(&philos->settings->critical_region);
		if (philos->settings->simu_done == true)
		{
			pthread_mutex_unlock(&philos->settings->critical_region);
			return (NULL);
		}
		pthread_mutex_unlock(&philos->settings->critical_region);
		if (all_alive(philos) == false)
		{
			pthread_mutex_lock(&philos->settings->critical_region);
			philos->settings->simu_done = true;
			pthread_mutex_unlock(&philos->settings->critical_region);
			return (NULL);
		}
		if (philos->settings->n_meals > -1 && all_eaten(philos))
		{
			pthread_mutex_lock(&philos->settings->critical_region);
			philos->settings->simu_done = true;
			pthread_mutex_unlock(&philos->settings->critical_region);
			return (NULL);
		}
	}
}
