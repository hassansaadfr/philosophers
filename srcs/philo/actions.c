#include "philo.h"

void	release_forks(t_philo *philo)
{
	int		right;

	right = philo->id + 1;
	if (right == philo->conf->nb)
		right = 0;
	pthread_mutex_unlock(&philo->conf->forks[philo->id]);
	pthread_mutex_unlock(&philo->conf->forks[right]);
}

void	take_forks(t_philo *philo)
{
	int		right;

	right = philo->id + 1;
	if (right == philo->conf->nb)
		right = 0;
	pthread_mutex_lock(&philo->conf->forks[philo->id]);
	print_state(philo, &philo->conf->printer, "has taken a fork");
	pthread_mutex_lock(&philo->conf->forks[right]);
	print_state(philo, &philo->conf->printer, "has taken a fork");
	if (!philo->ready)
		philo->ready = 1;
}

void	eating(t_philo *philo)
{
	if (philo->alive == 0)
		return ;
	philo->last_meal = utc_time_in_usec(now());
	print_state(philo, &philo->conf->printer, "is eating");
	sleep_time(philo->conf->eat);
	philo->eat++;
	release_forks(philo);
}

void	sleeping_and_thinking(t_philo *philo)
{
	if (philo->alive)
	{
		print_state(philo, &philo->conf->printer, "is sleeping");
		sleep_time(philo->conf->sleep);
	}
	if (philo->alive)
		print_state(philo, &philo->conf->printer, "is thinking");
}
