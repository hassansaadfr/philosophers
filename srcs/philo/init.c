#include "philo.h"

int	free_all_philos(t_philo **philos, int nb)
{
	int		i;

	i = 0;
	while (i < nb)
		free(philos[i++]);
	free(philos);
	return (0);
}

t_philo *create_one_philo(int id, t_conf *conf)
{
	t_philo *socrate;

	socrate = malloc(sizeof(t_philo));
	if (!socrate)
		return (NULL);
	socrate->id = id;
	socrate->ready = 0;
	socrate->alive = 1;
	socrate->conf = conf;
	socrate->eat = 0;
	socrate->last_meal = 0;
	return (socrate);
}

int	create_all_philo(t_conf *conf)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo) * conf->nb);
	i = 0;
	while (i < conf->nb)
	{
		philos[i] = create_one_philo(i, conf);
		if (!philos[i])
			return (free_all_philos(philos, i));
		if (pthread_create(&philos[i]->soul, NULL, seminary, (void *)philos[i]))
			return (free_all_philos(philos, i));
		while (i == 0 && !philos[0]->ready)
			usleep(100);
		i++;
	}
	i = 0;
	while (i < conf->nb)
		pthread_join(philos[i++]->soul, NULL);
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
