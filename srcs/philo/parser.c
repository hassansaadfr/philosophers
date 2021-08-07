#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

int	print_err(char *msg, int return_code)
{
	printf("%s\n", msg);
	return (return_code);
}

unsigned long	parse_long(char *str)
{
	unsigned long	out;
	int				i;

	out = 0;
	i = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		out = (out * 10) + str[i++] - '0';
	return (out);
}

static int	fill_struct(t_conf *conf, char **argv)
{
	conf->nb = parse_long(*argv++);
	if (conf->nb < 1)
		return (0);
	conf->die = parse_long(*argv++);
	conf->eat = parse_long(*argv++);
	conf->sleep = parse_long(*argv++);
	if (argv && *argv)
	{
		conf->eat_nb = parse_long(*argv);
		if (conf->eat_nb <= 0)
			return (0);
	}
	else
		conf->eat_nb = -1;
	if (conf->die <= 60 || conf->eat <= 60 || conf->sleep <= 60)
		return (0);
	return (1);
}

int	parse(char **argv, t_conf *conf)
{
	int		i;
	int		j;

	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] <= '9' && argv[i][j] >= '0'))
				return (print_err(PARSING_ISSUE, 0));
			j++;
		}
		i++;
	}
	if (fill_struct(conf, argv) == 0)
		return (print_err(PARSING_ISSUE, 0));
	if (pthread_mutex_init(&conf->printer, NULL))
		return (print_err(MUTEX_ERROR, 0));
	if (pthread_mutex_init(&conf->running_check, NULL))
		return (print_err(MUTEX_ERROR, 0));
	conf->start_time = now();
	conf->running = 1;
	return (1);
}
