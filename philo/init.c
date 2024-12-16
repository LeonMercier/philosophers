/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:51:57 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 15:01:58 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*allocate_forks(int n_philos)
{
	int				i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * n_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < n_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

static t_philo	*allocate_philos(t_settings *settings, pthread_mutex_t *forks)
{
	int		i;
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * settings->n_philos);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < settings->n_philos)
	{
		philos[i].id = i;
		philos[i].started_eating = -1;
		philos[i].times_eaten = 0;
		if (i == 0)
			philos[i].left = &forks[settings->n_philos - 1];
		else
			philos[i].left = &forks[i - 1];
		philos[i].right = &forks[i];
		philos[i].settings = settings;
		i++;
	}
	return (philos);
}

// after simulate(), things are freed because this is the end
// regardless of success
int	init(t_settings *settings)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;

	settings->dead_philo = -1;
	settings->simu_done = false;
	pthread_mutex_init(&settings->critical_region, NULL);
	forks = allocate_forks(settings->n_philos);
	if (!forks)
	{
		pthread_mutex_destroy(&settings->critical_region);
		return (1);
	}
	philos = allocate_philos(settings, forks);
	if (!philos)
	{
		pthread_mutex_destroy(&settings->critical_region);
		destroy_forks(forks, settings->n_philos);
		return (1);
	}
	simulate(philos);
	pthread_mutex_destroy(&settings->critical_region);
	destroy_forks(forks, settings->n_philos);
	free(philos);
	return (0);
}
