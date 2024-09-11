/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemercie <lemercie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:22:58 by lemercie          #+#    #+#             */
/*   Updated: 2024/09/11 17:11:21 by leon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_cur_time_ms()
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_mutex_print(long long time, int id, char *msg, pthread_mutex_t *mutex)
{
	int	pretty_id;

	pretty_id = id + 1;
	pthread_mutex_lock(mutex);
	printf("%lli %i %s\n", time, pretty_id, msg);
	pthread_mutex_unlock(mutex);
}

void	kill_philo(int id, t_settings *settings)
{
	pthread_mutex_lock(&settings->dead_philo_lock);
	settings->dead_philo = id;
	pthread_mutex_unlock(&settings->dead_philo_lock);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, id,
				"died", &settings->print_lock);
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
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
				"is eating", &settings->print_lock);
	philo->started_eating = get_cur_time_ms();
	usleep(settings->time_to_eat * 1000);
	philo->times_eaten++;
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

void	philo_sleep(t_settings *settings, t_philo *philo)
{
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
				"is sleeping", &settings->print_lock);
	usleep(settings->time_to_sleep * 1000);
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
	pthread_mutex_lock(philo->right);
	ft_mutex_print(get_cur_time_ms() - settings->start_time, philo->id,
				"has taken a fork", &settings->print_lock);
	pthread_mutex_unlock(&settings->pickup_lock);
}

// SUGGESTION
// odd philos pick left fork first
// even philos pick right fork first
// on first pick up delay between odd and even
//
// Dijkstra style
// whole pickup sequence is protected by a mutex
// but only lock that mutex if neither neighbour is eating
// BUT what if we read neighbour state and then it changes before we attempt to acquire the lock?
//
// TODO:always die with a single philo
//
// suggestion
// monitor in an infinte loop for time to die of each philo
// set dead flag on philo
// philo will check after every lock wait and quit if dead -> no extra messages will be printed even if 
// philo dies while waiting (and death msg is printed immediately)
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_settings *settings;

	philo = (t_philo *) *arg;
	settings = (t_settings *) arg;
	while (true)
	{
		think(settings, philo);
		pickup_forks(settings, philo);
		eat(settings, philo);
		philo_sleep(settings, philo);
		if (settings->dead_philo > -1)
			return (NULL);
		if (philo->times_eaten == settings->n_meals)
			return (NULL);
	}
	return (NULL);
}

bool	check_alive(t_settings *settings)
{
	int	i;

	i = 0;
	while (i < settings->n_philos)
	{
		if (settings->philos[i].started_eating == -1)
		{
			if ((get_cur_time_ms() - settings->philos[i].start_time) > settings->time_to_die)
				kill_philo(i, settings);
			return (false);
		}
		else
		{
			if ((get_cur_time_ms() - settings->philos[i].started_eating) > settings->time_to_die)
				kill_philo(i, settings);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	all_eaten(t_settings *settings)
{
	int	i;

	i = 0;
	while (i < settings->n_philos)
	{
		if (settings->philos[i].times_eaten < settings->n_meals)
			return (false);
		i++;
	}
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_settings	*settings;

	settings = (t_settings *) arg;
	while (true)
	{
		if (check_alive(settings) == false)
		{
			ft_mutex_print(get_cur_time_ms() - settings->start_time,
				  settings->dead_philo, "died", &settings->print_lock);
			return (NULL);
		}
		if (all_eaten(settings))
		{
			return (NULL);
		}
	}
}
 // this doesnt work; better just have an array of philos with everything in them
// just remember to make the mutex members be pointers to a single variable
// so get rid of t_settings
void	simulate(t_settings *settings)
{
	int			i;
	pthread_t	*threads;
	pthread_t	monitor_thd;
	t_two_ptr	routine_args;

	threads = malloc(sizeof(pthread_t) * settings->n_philos);
	if (!threads)
	{
		// fail malloc
	}
	pthread_create(&monitor_thd, NULL, &monitor_routine, settings);
	i = 0;
	routine_args.settings = settings;
	while (i < settings->n_philos)
	{
		ttings_and_philo[1] = (void *) &settings->philos[i];
		//pthread_create(&threads[i], NULL, &philo_routine, &settings->philos[i]);
		pthread_create(&threads[i], NULL, &philo_routine, &settings_and_philo);
		i++;
	}
	pthread_join(monitor_thd, NULL);
	i = 0;
	while (i < settings->n_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	return ;
}

// TODO IDs start at 1
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
