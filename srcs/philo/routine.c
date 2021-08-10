/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsaadaou <hsaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 20:00:22 by hsaadaou          #+#    #+#             */
/*   Updated: 2021/08/10 20:00:36 by hsaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine(t_philo *socrate)
{
	int		alive;
	int		running;

	alive = 1;
	running = 1;
	while (alive && running)
	{
		alive = is_alive(socrate);
		running = is_running(socrate);
		take_forks(socrate);
		eating(socrate);
		release_forks(socrate);
		sleeping_and_thinking(socrate);
	}
}

void	*seminary(void *data)
{
	t_philo		*socrate;
	pthread_t	hades;
	int			alive;
	int			running;

	alive = 1;
	running = 1;
	socrate = (t_philo *)data;
	if (pthread_create(&hades, NULL, purgatory, (void *)socrate))
		return (NULL);
	if (socrate->conf->nb == 1)
	{
		pthread_mutex_lock(&socrate->conf->forks[0]);
		print_state(socrate, &socrate->conf->printer, "has take a fork");
		pthread_mutex_unlock(&socrate->conf->forks[0]);
		while (alive)
			alive = is_alive(socrate);
	}
	else
		routine(socrate);
	pthread_join(hades, NULL);
	return (NULL);
}
