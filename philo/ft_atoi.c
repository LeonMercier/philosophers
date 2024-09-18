/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:45:05 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/18 13:28:27 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_valid_number(char *s)
{
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (false);
		s++;
	}
	return (true);
}
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
