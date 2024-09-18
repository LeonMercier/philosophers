/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:51:57 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/18 11:37:01 by lemercie         ###   ########.fr       */
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
		philos[i].eating = false;
		philos[i].start_time = get_cur_time_ms();
		printf("%lli\n", philos[i].start_time);
		philos[i].started_eating = -1;
		philos[i].times_eaten = 0;
		if (i == 0)
		{
			philos[i].left = &forks[settings->n_philos - 1];
			philos[i].right = &forks[i];
		}
		else if (i == settings->n_philos - 1)
		{
			philos[i].left = &forks[i];
			philos[i].right = &forks[0];
		}
		else
		{
			philos[i].left = &forks[i - 1];
			philos[i].right = &forks[i];
		}
		philos[i].settings = settings;
		i++;
	}
	return (philos);
}

int init(t_settings *settings)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;

	forks = allocate_forks(settings->n_philos); // NULL can arrive here
	if (!forks)
		return (1);
	philos = allocate_philos(settings, forks); // NULL can arrive here
	if (!philos)
	{
		// forks -> destroy mutexes
		return (1);
	}
	simulate(philos);
	return (0);
}
