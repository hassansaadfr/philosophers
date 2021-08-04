#include "philo.h"

void	*purgatory(void *data)
{
	t_philo *socrate;

	socrate = (t_philo *)data;
	while (socrate->alive)
	{
		if (socrate->eat > 0 && utc_time_in_usec(now()) - socrate->last_meal > socrate->conf->die)
		{
			socrate->alive = 0;
			print_state(socrate, &socrate->conf->printer, "died");
			break ;
		}
		usleep(500);
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
	while (socrate->alive)
	{
		take_forks(socrate);
		eating(socrate);
		sleeping_and_thinking(socrate);
	}
	pthread_join(hades, NULL);
	return (NULL);
}
