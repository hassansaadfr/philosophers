#include "philo.h"

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
		pthread_mutex_lock(&philo->conf->forks[philo->id]);
		print_state(philo, &philo->conf->printer, "has taken a fork");
		pthread_mutex_lock(&philo->conf->forks[right]);
		print_state(philo, &philo->conf->printer, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->conf->forks[right]);
		print_state(philo, &philo->conf->printer, "has taken a fork");
		pthread_mutex_lock(&philo->conf->forks[philo->id]);
		print_state(philo, &philo->conf->printer, "has taken a fork");
	}
}

void	eating(t_philo *philo)
{
	if (!philo->alive && !philo->conf->running)
		return ;
	pthread_mutex_lock(&philo->meal_check);
	philo->last_meal = utc_time_in_usec(now());
	pthread_mutex_unlock(&philo->meal_check);
	pthread_mutex_lock(&philo->eat_check);
	philo->eat++;
	pthread_mutex_unlock(&philo->eat_check);
	print_state(philo, &philo->conf->printer, "is eating");
	sleep_time(philo->conf->eat);
	release_forks(philo);
}

void	sleeping_and_thinking(t_philo *philo)
{
	int	alive;

	pthread_mutex_lock(&philo->alive_check);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_check);
	if (philo->conf->nb == 1)
		return ;
	if (alive && philo->conf->running)
	{
		print_state(philo, &philo->conf->printer, "is sleeping");
		sleep_time(philo->conf->sleep);
	}
	if (alive && philo->conf->running)
		print_state(philo, &philo->conf->printer, "is thinking");
}
