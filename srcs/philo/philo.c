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

void	*purgatory(void *data)
{
	t_philo	*socrate;
	int		eat;
	int		alive;
	int		running;
	unsigned long		last_meal;

	alive = 1;
	running = 1;
	socrate = (t_philo *)data;
	while (alive && running)
	{
		alive = is_alive(socrate);
		running = is_running(socrate);
		pthread_mutex_lock(&socrate->eat_check);
		eat = socrate->eat;
		pthread_mutex_unlock(&socrate->eat_check);
		pthread_mutex_lock(&socrate->meal_check);
		last_meal = socrate->last_meal;
		pthread_mutex_unlock(&socrate->meal_check);
		if (eat > 0 && (utc_time_in_usec(now()) - last_meal > socrate->conf->die || (eat >= socrate->conf->eat_nb && socrate->conf->eat_nb != -1)))
		{
			pthread_mutex_lock(&socrate->alive_check);
			socrate->alive = 0;
			pthread_mutex_unlock(&socrate->alive_check);
			if (eat < socrate->conf->eat_nb || socrate->conf->eat_nb == -1)
			{
				print_state(socrate, &socrate->conf->printer, "died");
				pthread_mutex_lock(&socrate->conf->running_check);
				socrate->conf->running = 0;
				pthread_mutex_unlock(&socrate->conf->running_check);
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
	int			alive;
	int			running;

	alive = 1;
	running = 1;
	socrate = (t_philo *)data;
	if (pthread_create(&hades, NULL, purgatory, (void *)socrate))
		return (NULL);
	while (alive && running)
	{
		alive = is_alive(socrate);
		running = is_running(socrate);
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
