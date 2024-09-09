/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/09 17:51:13 by leon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>

void	*philo_routine(void *arg)
{
	printf("I am philosopher %i\n", ((t_philo *) arg)->id);
	return (NULL);
}

void	simulate(t_settings *settings, t_philo *philos)
{
	int			i;
	pthread_t	*threads;

	threads = malloc(sizeof(pthread_t) * settings->n_philos);
	if (!threads)
	{
		// fail malloc
	}
	i = 0;
	while (i < settings->n_philos)
	{
		pthread_create(&threads[i], NULL, &philo_routine, &philos[i]);
		i++;
	}
	i = 0;
	while (i < settings->n_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_settings settings;

	if (argc < 5 || argc > 6)
	{
		printf("Error: need 4 or 5 arguments\n");
		return (1);
	}
	settings.n_philos = ft_atoi(argv[1]);
	settings.time_to_die = ft_atoi(argv[2]);
	settings.time_to_eat = ft_atoi(argv[3]);
	settings.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		settings.n_meals = ft_atoi(argv[5]);
	else
		settings.n_meals = -1;
	printf("%i, %i, %i, %i, %i\n", settings.n_philos, settings.time_to_die, 
		settings.time_to_eat, settings.time_to_sleep, settings.n_meals);
	if (init(&settings))
		return (1);
	return (0);
}
