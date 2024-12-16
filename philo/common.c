/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:39:06 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 15:41:13 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	simu_done(t_settings *settings)
{
	pthread_mutex_lock(&settings->critical_region);
	if (settings->simu_done)
	{
		pthread_mutex_unlock(&settings->critical_region);
		return (true);
	}
	pthread_mutex_unlock(&settings->critical_region);
	return (false);
}

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
