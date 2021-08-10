/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsaadaou <hsaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 19:49:32 by hsaadaou          #+#    #+#             */
/*   Updated: 2021/08/10 20:09:55 by hsaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_alive(t_philo *philo)
{
	int		ret;

	ret = 1;
	pthread_mutex_lock(&philo->alive_check);
	ret = philo->alive;
	pthread_mutex_unlock(&philo->alive_check);
	return (ret);
}

int	is_running(t_philo *philo)
{
	int		ret;

	ret = 1;
	pthread_mutex_lock(&philo->conf->running_check);
	ret = philo->conf->running;
	pthread_mutex_unlock(&philo->conf->running_check);
	return (ret);
}

int	is_sated(t_philo *socrate)
{
	int	eat;
	int	is_sated;

	eat = 0;
	is_sated = 0;
	if (socrate->conf->eat_nb != -1)
	{
		pthread_mutex_lock(&socrate->eat_check);
		eat = socrate->eat;
		pthread_mutex_unlock(&socrate->eat_check);
		if (eat == socrate->conf->eat_nb - 1)
		{
			pthread_mutex_lock(&socrate->alive_check);
			socrate->alive = 0;
			pthread_mutex_unlock(&socrate->alive_check);
			is_sated = 1;
		}
	}
	return (is_sated);
}

int	is_eating_at_time(t_philo *socrate)
{
	unsigned long		last_meal;
	int					is_alive;

	is_alive = 1;
	pthread_mutex_lock(&socrate->meal_check);
	last_meal = socrate->last_meal;
	pthread_mutex_unlock(&socrate->meal_check);
	if ((utc_time_in_usec(now()) - last_meal > socrate->conf->die))
	{
		pthread_mutex_lock(&socrate->alive_check);
		socrate->alive = 0;
		pthread_mutex_unlock(&socrate->alive_check);
		print_state(socrate, &socrate->conf->printer, "died");
		pthread_mutex_lock(&socrate->conf->running_check);
		socrate->conf->running = 0;
		pthread_mutex_unlock(&socrate->conf->running_check);
		is_alive = 0;
	}
	return (is_alive);
}

void	*purgatory(void *data)
{
	t_philo				*socrate;
	int					alive;
	int					running;

	alive = 1;
	running = 1;
	socrate = (t_philo *)data;
	while (alive && running)
	{
		alive = is_alive(socrate);
		running = is_running(socrate);
		if (is_sated(socrate))
			break ;
		if (is_eating_at_time(socrate) == 0)
			break ;
		usleep(100);
	}
	return (NULL);
}
