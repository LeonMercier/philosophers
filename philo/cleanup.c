/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:22:15 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/16 16:32:40 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(pthread_mutex_t *forks, int index)
{
	if (!forks)
		return ;
	while (index >= 0)
	{
		pthread_mutex_destroy(&forks[index]);
		index--;
	}
	free(forks);
}

void	join_threads(pthread_t *threads, int index)
{
	if (!threads)
		return ;
	while (index >= 0)
	{
		pthread_join(threads[index], NULL);
		index--;
	}
	free(threads);
}
