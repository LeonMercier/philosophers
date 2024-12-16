/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:39:06 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 11:34:46 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
// dead_philo == -2 => death has already been printed
void	ft_mutex_print(long long time, t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->settings->critical_region);
	if (philo->settings->dead_philo >= 0)
	{
		printf("%lli %i %s\n", time, philo->settings->dead_philo + 1, "died");
		philo->settings->dead_philo = -2;
		pthread_mutex_unlock(&philo->settings->critical_region);
		return ;
	}
	else if (philo->settings->dead_philo == -2)
	{
		pthread_mutex_unlock(&philo->settings->critical_region);
		return ;
	}
	else
		printf("%lli %i %s\n", time, philo->id + 1, msg);
	pthread_mutex_unlock(&philo->settings->critical_region);
}
*/

// dead_philo == -2 => death has already been printed
void	ft_mutex_print(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->settings->critical_region);
	if (philo->settings->dead_philo >= 0)
	{
		printf("%lli %i %s\n", get_cur_time_ms() - philo->start_time,
			philo->settings->dead_philo + 1, "died");
		philo->settings->dead_philo = -2;
		pthread_mutex_unlock(&philo->settings->critical_region);
		return ;
	}
	else if (philo->settings->dead_philo == -2)
	{
		pthread_mutex_unlock(&philo->settings->critical_region);
		return ;
	}
	else
		printf("%lli %i %s\n", get_cur_time_ms() - philo->start_time,
			philo->id + 1, msg);
	pthread_mutex_unlock(&philo->settings->critical_region);
}
