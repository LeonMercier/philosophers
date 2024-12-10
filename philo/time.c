/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:13:40 by lemercie          #+#    #+#             */
/*   Updated: 2024/12/10 16:13:57 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_cur_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("Error in gettimeofday()\n");
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
