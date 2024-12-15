/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/15 18:07:54 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_settings(t_settings *settings, int argc, char **argv)
{
	settings->n_philos = ft_atoi(argv[1]);
	settings->time_to_die = ft_atoi(argv[2]);
	settings->time_to_eat = ft_atoi(argv[3]);
	settings->time_to_sleep = ft_atoi(argv[4]);
	settings->dead_philo = -1;
	settings->simu_done = false;
	pthread_mutex_init(&settings->critical_region, NULL);
	if (argc == 6)
		settings->n_meals = ft_atoi(argv[5]);
	else
		settings->n_meals = -1;
}

static int	validate_args(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Error: need 4 or 5 arguments\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (is_valid_number(argv[i]) == false)
		{
			printf("Error: argument is not a positive number\n");
			return (1);
		}
		i++;
	}
	return (0);
}

// TODO: died message has to be the last thing printed
// TODO: 5 800 200 200 is NOT allowed to die
int	main(int argc, char **argv)
{
	t_settings settings;

	if (validate_args(argc, argv) == 1)
		return (1);
	init_settings(&settings, argc, argv);
	if (init(&settings))
		return (1);
	return (0);
}
