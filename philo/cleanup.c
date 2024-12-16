/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:22:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 16:16:32 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(pthread_mutex_t *forks, int num)
{
	if (!forks)
		return ;
	while (num > 0)
	{
		pthread_mutex_destroy(&forks[num - 1]);
		num--;
	}
	free(forks);
}

void	join_threads(pthread_t *threads, int num)
{
	if (!threads)
		return ;
	while (num >= 0)
	{
		pthread_join(threads[num], NULL);
		num--;
	}
	free(threads);
}
