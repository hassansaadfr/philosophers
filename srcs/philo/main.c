#include "philo.h"

int	main(int argc, char **argv)
{
	t_conf			conf;

	if (argc == 5 || argc == 6)
	{
		argv++;
		if (!parse(argv, &conf))
			return (1);
		if (!create_table(&conf))
			return (0);
		free_table(&conf);
	}
	else
		return (print_err(INVALID_ARGS, 1));
	return (0);
}
