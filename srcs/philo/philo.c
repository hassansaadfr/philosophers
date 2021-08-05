#include "philo.h"

void	*purgatory(void *data)
{
	t_philo	*socrate;
	int		eat;
	int		alive;

	alive = 1;
	socrate = (t_philo *)data;
	while (alive && socrate->conf->running)
	{
		pthread_mutex_lock(&socrate->alive_check);
		alive = socrate->alive;
		pthread_mutex_unlock(&socrate->alive_check);
		pthread_mutex_lock(&socrate->eat_check);
		eat = socrate->eat;
		pthread_mutex_unlock(&socrate->eat_check);
		if (eat > 0 && (utc_time_in_usec(now()) - socrate->last_meal > socrate->conf->die || (eat >= socrate->conf->eat_nb && socrate->conf->eat_nb != -1)))
		{
			pthread_mutex_lock(&socrate->alive_check);
			socrate->alive = 0;
			pthread_mutex_unlock(&socrate->alive_check);
			if (eat < socrate->conf->eat_nb || socrate->conf->eat_nb == -1)
			{
				print_state(socrate, &socrate->conf->printer, "died");
				socrate->conf->running = 0;
			}
			break ;
		}
		usleep(100);
	}
	return (NULL);
}

void	*seminary(void *data)
{
	t_philo		*socrate;
	pthread_t	hades;

	socrate = (t_philo *)data;
	if (pthread_create(&hades, NULL, purgatory, (void *)socrate))
		return (NULL);
	while (socrate->alive && socrate->conf->running)
	{
		take_forks(socrate);
		if (socrate->conf->nb > 1)
		{
			eating(socrate);
			sleeping_and_thinking(socrate);
		}
		else
			sleep_time(socrate->conf->eat);
	}
	pthread_join(hades, NULL);
	return (NULL);
}
