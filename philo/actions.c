/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:16:54 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 15:17:46 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_wait(t_settings *settings, long long int to_wait_ms)
{
	long long int	end_time;

	end_time = get_cur_time_ms() + to_wait_ms;
	while (get_cur_time_ms() < end_time)
	{
		pthread_mutex_lock(&settings->critical_region);
		if (settings->simu_done)
		{
			pthread_mutex_unlock(&settings->critical_region);
			return (1);
		}
		pthread_mutex_unlock(&settings->critical_region);
		usleep(500);
	}
	return (0);
}

void	think(t_philo *philo)
{
	ft_mutex_print(philo, "is thinking");
}

void	eat(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(philo, "is eating");
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
	ft_mutex_print(philo, "is sleeping");
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
	ft_mutex_print(philo, "has taken a fork");
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->right);
	else
		pthread_mutex_lock(philo->left);
	ft_mutex_print(philo, "has taken a fork");
}
