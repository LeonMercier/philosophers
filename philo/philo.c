/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 14:25:46 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_settings(t_settings *settings, int argc, char **argv)
{
	int	err_atoi;
	int	tmp;

	err_atoi = 0;
	tmp = ft_atoi_safe(argv[1], &err_atoi);
	if (err_atoi || tmp < 0)
		return (1);
	settings->n_philos = tmp;
	tmp = ft_atoi_safe(argv[2], &err_atoi);
	if (err_atoi || tmp < 0)
		return (1);
	settings->time_to_die = tmp;
	tmp = ft_atoi_safe(argv[3], &err_atoi);
	if (err_atoi || tmp < 0)
		return (1);
	settings->time_to_eat = tmp;
	tmp = ft_atoi_safe(argv[4], &err_atoi);
	if (err_atoi || tmp < 0)
		return (1);
	settings->time_to_sleep = tmp;
	if (argc == 6)
	{
		tmp = ft_atoi_safe(argv[5], &err_atoi);
		if (err_atoi || tmp < 0)
			return (1);
		settings->n_meals = tmp;
	}
	else
		settings->n_meals = -1;
	settings->dead_philo = -1;
	settings->simu_done = false;
	pthread_mutex_init(&settings->critical_region, NULL);
	return (0);
}

static int	validate_args(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: need 4 or 5 arguments\n");
		return (1);
	}
	return (0);
}

// NOTE: 5 800 200 200 is NOT allowed to die
int	main(int argc, char **argv)
{
	t_settings settings;

	if (validate_args(argc) == 1)
		return (1);
	if (init_settings(&settings, argc, argv) != 0)
	{
		printf("Error: argument is not a supported number\n");
		return (1);
	}
	if (settings.n_philos < 1)
	{
		printf("Error: need at least one philosopher\n");
		return (1);
	}
	if (settings.n_meals == 0)
		return (0);
	if (init(&settings))
	{
		pthread_mutex_destroy(&settings.critical_region);
		return (1);
	}
	pthread_mutex_destroy(&settings.critical_region);
	return (0);
}
