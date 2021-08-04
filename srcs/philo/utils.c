#include "philo.h"

long long int	utc_time_in_usec(struct timeval time)
{
	return (((long long int)time.tv_sec * 1000) + (time.tv_usec / 1000));
}

struct timeval	now(void)
{
	struct timeval	n;

	gettimeofday(&n, NULL);
	return (n);
}

long long int	elapsed_time(struct timeval start)
{
	return (utc_time_in_usec(now()) - utc_time_in_usec(start));
}

void	sleep_time(int time)
{
	long long int	n;

	n = utc_time_in_usec(now());
	while (utc_time_in_usec(now()) - n < time)
		usleep(100);
}

void	print_state(t_philo *philo, pthread_mutex_t *printer, char *state)
{
	pthread_mutex_lock(printer);
	if (philo->conf->running)
		printf("%lld\t%d\t%s\n", elapsed_time(philo->conf->start_time), philo->id + 1, state);
	pthread_mutex_unlock(printer);
}
