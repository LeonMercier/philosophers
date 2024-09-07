/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:45:05 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/07 13:14:32 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// this bad boy doesn't do any error checking
int	ft_atoi(char *s)
{
	int	num;
	int	neg;

	num = 0;
	neg = 1;
	if (*s == '-')
	{
		neg = -1;
		s++;
	}
	while (*s)
	{
		num = (num * 10) + (*s - '0');
		s++;
	}
	return (num * neg);
}
