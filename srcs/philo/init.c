/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsaadaou <hsaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 19:49:40 by hsaadaou          #+#    #+#             */
/*   Updated: 2021/08/10 20:15:47 by hsaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_monitors(t_philo **philos, t_conf *conf)
{
	int		i;
	int		eat_count;

	i = 0;
	eat_count = 0;
	while (i < conf->nb)
	{
		pthread_join(philos[i]->soul, NULL);
		if (philos[i]->eat == conf->eat_nb)
			eat_count++;
		i++;
	}
	if (eat_count == conf->nb)
		printf("Each philos eat %lld time\n", conf->eat_nb);
}

int	free_all_philos(t_philo **philos, int nb)
{
	int		i;

	i = 0;
	while (i < nb)
		free(philos[i++]);
	free(philos);
	return (0);
}

t_philo	*create_one_philo(int id, t_conf *conf)
{
	t_philo	*socrate;

	socrate = malloc(sizeof(t_philo));
	if (!socrate)
		return (NULL);
	socrate->id = id;
	socrate->alive = 1;
	socrate->conf = conf;
	socrate->eat = 0;
	socrate->last_meal = utc_time_in_usec(now());
	if (pthread_mutex_init(&socrate->eat_check, NULL))
		return (NULL);
	if (pthread_mutex_init(&socrate->meal_check, NULL))
		return (NULL);
	if (pthread_mutex_init(&socrate->alive_check, NULL))
		return (NULL);
	return (socrate);
}

int	create_all_philo(t_conf *conf)
{
	t_philo	**philos;
	int		i;
	int		eat_count;

	eat_count = 0;
	philos = malloc(sizeof(t_philo) * conf->nb);
	i = 0;
	while (i < conf->nb)
	{
		philos[i] = create_one_philo(i, conf);
		if (!philos[i])
			return (free_all_philos(philos, i));
		if (pthread_create(&philos[i]->soul, NULL, seminary, (void *)philos[i]))
			return (free_all_philos(philos, i));
		usleep(100);
		i++;
	}
	i = 0;
	wait_monitors(philos, conf);
	free_all_philos(philos, conf->nb);
	return (1);
}

int	create_table(t_conf *conf)
{
	pthread_mutex_t		*forks;
	int					i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * conf->nb);
	while (i < conf->nb)
	{
		if (pthread_mutex_init(&forks[i], NULL))
		{
			free(forks);
			return (0);
		}
		i++;
	}
	conf->forks = forks;
	return (create_all_philo(conf));
}
