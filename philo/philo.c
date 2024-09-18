/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/18 14:19:28 by lemercie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

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

void	ft_mutex_print(long long time, int id, char *msg,
			pthread_mutex_t *mutex)
{
	int	pretty_id;

	pretty_id = id + 1;
	pthread_mutex_lock(mutex);
	printf("%lli %i %s\n", time, pretty_id, msg);
	pthread_mutex_unlock(mutex);
}

bool	all_alive(void *arg)
{
	t_settings	*settings;

	settings = (t_settings *) arg;
	pthread_mutex_lock(&settings->dead_philo_lock);
	if (settings->dead_philo == -1)
	{
		pthread_mutex_unlock(&settings->dead_philo_lock);
		return (true);
	}
	pthread_mutex_unlock(&settings->dead_philo_lock);
	return (false);
}

void	think(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
		"is thinking", &settings->print_lock);
	usleep(100);
}

void	eat(t_settings *settings, t_philo *philo)
{
	philo->eating = true;
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
		"is eating", &settings->print_lock);
	philo->started_eating = get_cur_time_ms();
	usleep(settings->time_to_eat * 1000);
	philo->times_eaten++;
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	philo->eating = false;
}

void	philo_sleep(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
		"is sleeping", &settings->print_lock);
	usleep(settings->time_to_sleep * 1000);
}

void	kill_philo(int id, t_settings *settings)
{
	pthread_mutex_lock(&settings->dead_philo_lock);
	settings->dead_philo = id;
	pthread_mutex_unlock(&settings->dead_philo_lock);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, id,
		"died", &settings->print_lock);
}

// philo will be blocked in this function until they can pick up both forks 
// conceptually they will be thinking
// TODO: how is the philo gonna starve while waiting here?
void	pickup_forks(t_settings *settings, t_philo *philo)
{
	pthread_mutex_lock(&settings->pickup_lock);
	pthread_mutex_lock(philo->left);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
		"has taken a fork", &settings->print_lock);
	if (settings->n_philos == 1)
	{
		usleep(settings->time_to_die);
		kill_philo(philo->id, settings);
		return ;
	}
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
		"die", &settings->print_lock);
	pthread_mutex_lock(philo->right);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
		"has taken a fork", &settings->print_lock);
	pthread_mutex_unlock(&settings->pickup_lock);
	usleep(100);
}

// SUGGESTION
// odd philos pick left fork first
// even philos pick right fork first
// on first pick up delay between odd and even
//
// Dijkstra style
// whole pickup sequence is protected by a mutex
// but only lock that mutex if neither neighbour is eating
// BUT what if we read neighbour state and then it changes before we attempt to
// 		acquire the lock?
//
//
// suggestion
// monitor in an infinte loop for time to die of each philo
// set dead flag on philo
// philo will check after every lock wait and quit if dead
// 		-> no extra messages will be printed even if 
// philo dies while waiting (and death msg is printed immediately)
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (true)
	{
		if (!all_alive(philo->settings))
			return (NULL);
		think(philo->settings, philo);
		if (!all_alive(philo->settings))
			return (NULL);
		pickup_forks(philo->settings, philo);
		if (!all_alive(philo->settings))
			return (NULL);
		eat(philo->settings, philo);
		if (!all_alive(philo->settings))
			return (NULL);
		philo_sleep(philo->settings, philo);
		if (philo->times_eaten == philo->settings->n_meals)
			return (NULL);
	}
	return (NULL);
}



bool	check_alive(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->settings->n_philos)
	{
		if (philos[i].started_eating == -1)
		{
			if ((get_cur_time_ms() - philos[i].start_time)
				> philos->settings->time_to_die)
			{
				kill_philo(i, philos->settings);
				return (false);
			}
		}
		else
		{
			if ((get_cur_time_ms() - philos[i].started_eating)
				> philos->settings->time_to_die)
			{
				kill_philo(i, philos->settings);
				return (false);
			}
		}
		i++;
	}
	return (true);
}

bool	all_eaten(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->settings->n_philos)
	{
		if (philos[i].times_eaten < philos->settings->n_meals)
			return (false);
		i++;
	}
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *) arg;
	while (true)
	{
		if (check_alive(philos) == false)
		{
			return (NULL);
		}
		if (philos->settings->n_meals > -1 && all_eaten(philos))
		{
			return (NULL);
		}
	}
}

void	simulate(t_philo *philos)
{
	int			i;
	pthread_t	*threads;
	pthread_t	monitor_thd;

	threads = malloc(sizeof(pthread_t) * philos->settings->n_philos);
	if (!threads)
	{
		// fail malloc
	}
	pthread_create(&monitor_thd, NULL, &monitor_routine, philos);
	i = 0;
	while (i < philos->settings->n_philos)
	{
		pthread_create(&threads[i], NULL, &philo_routine, &philos[i]);
		i++;
	}
	pthread_join(monitor_thd, NULL);
	i = 0;
	while (i < philos->settings->n_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	return ;
}
// TODO: died message has to be the last thing printed
// TODO: 5 310 100 100 is NOT allowed to die
int	main(int argc, char **argv)
{
	t_settings settings;
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
	settings.n_philos = ft_atoi(argv[1]);
	settings.time_to_die = ft_atoi(argv[2]);
	settings.time_to_eat = ft_atoi(argv[3]);
	settings.time_to_sleep = ft_atoi(argv[4]);
	settings.dead_philo = -1;
	settings.start_time = get_cur_time_ms();
	pthread_mutex_init(&settings.dead_philo_lock, NULL);
	pthread_mutex_init(&settings.print_lock, NULL);
	pthread_mutex_init(&settings.pickup_lock, NULL);
	if (argc == 6)
		settings.n_meals = ft_atoi(argv[5]);
	else
		settings.n_meals = -1;
	if (init(&settings))
		return (1);
	return (0);
}
