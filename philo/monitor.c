/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:36:23 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/10 17:23:17 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_alive(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->settings->n_philos)
	{
		if (philos[i].started_eating == -1)
		{
			if ((get_cur_time_ms() - philos[i].start_time)
				> philos->settings->time_to_die)
			{
				kill_philo(&philos[i], philos->settings);
				return (false);
			}
		}
		else
		{
			if ((get_cur_time_ms() - philos[i].started_eating)
				> philos->settings->time_to_die)
			{
				printf("KILL\n");
				kill_philo(&philos[i], philos->settings);
				return (false);
			}
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
		if (philos[i].times_eaten < philos->settings->n_meals)
		{
			return (false);
		}
		i++;
	}
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *) arg;
	while (true)
	{
		usleep(100); // HMMM
		if (check_alive(philos) == false)
		{
			philos->settings->simu_done = true;
			ft_mutex_print(get_cur_time_ms() - philos->settings->start_time,
				   philos, "lol");
			return (NULL);
		}
		if (philos->settings->n_meals > -1 && all_eaten(philos))
		{
			philos->settings->simu_done = true;
			return (NULL);
		}
	}
}
