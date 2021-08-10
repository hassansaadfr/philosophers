#include "philo.h"

int	take_fork(pthread_mutex_t *fork, t_philo *philo)
{
	if (is_alive(philo) == 0 || is_running(philo) == 0)
		return (0);
	pthread_mutex_lock(fork);
	print_state(philo, &philo->conf->printer, "has taken a fork");
	return (1);
}

void	release_forks(t_philo *philo)
{
	int		right;

	right = philo->id + 1;
	if (philo->conf->nb == 1)
		pthread_mutex_unlock(&philo->conf->forks[philo->id]);
	if (right == philo->conf->nb)
		right = 0;
	if ((philo->id + 1) % 2 == 0)
	{
		pthread_mutex_unlock(&philo->conf->forks[right]);
		pthread_mutex_unlock(&philo->conf->forks[philo->id]);
	}
	else
	{
		pthread_mutex_unlock(&philo->conf->forks[philo->id]);
		pthread_mutex_unlock(&philo->conf->forks[right]);
	}
}

void	take_forks(t_philo *philo)
{
	int		right;

	right = philo->id + 1;
	if (right == philo->conf->nb)
		right = 0;
	if (philo->conf->nb == 1)
	{
		pthread_mutex_lock(&philo->conf->forks[philo->id]);
		print_state(philo, &philo->conf->printer, "has taken a fork");
		pthread_mutex_lock(&philo->meal_check);
		philo->last_meal = utc_time_in_usec(now());
		pthread_mutex_unlock(&philo->meal_check);
	}
	else if ((philo->id + 1) % 2 == 0)
	{
		take_fork(&philo->conf->forks[philo->id], philo);
		if (take_fork(&philo->conf->forks[right], philo) == 0)
			pthread_mutex_unlock(&philo->conf->forks[philo->id]);
	}
	else
	{
		take_fork(&philo->conf->forks[right], philo);
		if (take_fork(&philo->conf->forks[philo->id], philo) == 0)
			pthread_mutex_unlock(&philo->conf->forks[right]);
	}
}

void	eating(t_philo *philo)
{
	if (!is_alive(philo) && !is_running(philo))
		return ;
	pthread_mutex_lock(&philo->meal_check);
	philo->last_meal = utc_time_in_usec(now());
	pthread_mutex_unlock(&philo->meal_check);
	pthread_mutex_lock(&philo->eat_check);
	philo->eat++;
	pthread_mutex_unlock(&philo->eat_check);
	print_state(philo, &philo->conf->printer, "is eating");
	sleep_time(philo->conf->eat);
}

void	sleeping_and_thinking(t_philo *philo)
{
	if (philo->conf->nb == 1)
		return ;
	if (is_alive(philo) && is_running(philo))
	{
		print_state(philo, &philo->conf->printer, "is sleeping");
		sleep_time(philo->conf->sleep);
	}
	if (is_alive(philo) && is_running(philo))
		print_state(philo, &philo->conf->printer, "is thinking");
}
