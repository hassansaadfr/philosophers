#ifndef PHILO_H

# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# include <pthread.h>
# include <sys/time.h>

# define INVALID_ARGS "Invalid args."
# define MUTEX_ERROR "Error during mutex initialisation."
# define PARSING_ISSUE "Bad arguments."


typedef struct s_conf {
	long long			nb;
	unsigned long		die;
	unsigned long		eat;
	unsigned long		sleep;
	long long			eat_nb;
	struct timeval		start_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		printer;
}	t_conf;

typedef struct s_philo {
	int				id;
	pthread_t		soul;
	int				ready;
	int				alive;
	int				eat;
	unsigned long	last_meal;
	t_conf			*conf;
}	t_philo;

int					parse(char **argv, t_conf *conf);
int					print_err(char *msg, int return_code);
int					ft_strlen(char *str);
struct timeval		now(void);
long long int		elapsed_time(struct timeval start);
long long int		utc_time_in_usec(struct timeval time);
void				sleep_time(int time);
unsigned long		get_time(void);
void				print_state(t_philo *philo, pthread_mutex_t *printer, char *state);

void				take_forks(t_philo *philo);
void				release_forks(t_philo *philo);
void				eating(t_philo *philo);
void				sleeping_and_thinking(t_philo *philo);

void				*seminary(void *data);

int					create_table(t_conf *conf);

#endif
