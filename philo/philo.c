/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/09 17:20:31 by leon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t	*allocate_forks(int n_philos)
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

t_philo	*allocate_philos(t_settings *settings, pthread_mutex_t *forks)
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
		philos[i].alive = true;
		philos[i].times_eaten = 0;
		philos[i].settings = settings;
		philos[i].forks = forks;
		i++;
	}
	return (philos);
}

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

int init(t_settings *settings)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;

	forks = allocate_forks(settings->n_philos); // NULL can arrive here
	philos = allocate_philos(settings, forks); // NULL can arrive here
	if (forks && philos)
	{
		simulate(settings, philos);
		return (0);
	}
	// if forks, free forks, if philos free philos
	return (1);
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
