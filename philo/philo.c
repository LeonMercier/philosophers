/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 16:13:07 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_settings(t_settings *settings, int argc, char **argv)
{
	int	err_atoi;

	err_atoi = 0;
	settings->n_philos = ft_atoi_safe(argv[1], &err_atoi);
	if (err_atoi || settings->n_philos < 0)
		return (1);
	settings->time_to_die = ft_atoi_safe(argv[2], &err_atoi);
	if (err_atoi || settings->time_to_die < 0)
		return (1);
	settings->time_to_eat = ft_atoi_safe(argv[3], &err_atoi);
	if (err_atoi || settings->time_to_eat < 0)
		return (1);
	settings->time_to_sleep = ft_atoi_safe(argv[4], &err_atoi);
	if (err_atoi || settings->time_to_sleep < 0)
		return (1);
	if (argc == 6)
	{
		settings->n_meals = ft_atoi_safe(argv[5], &err_atoi);
		if (err_atoi || settings->n_meals < 0)
			return (1);
	}
	else
		settings->n_meals = -1;
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

int	main(int argc, char **argv)
{
	t_settings	settings;

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
		return (1);
	}
	return (0);
}
