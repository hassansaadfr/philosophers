/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsaadaou <hsaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 19:49:42 by hsaadaou          #+#    #+#             */
/*   Updated: 2021/08/10 19:54:24 by hsaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		free(conf.forks);
	}
	else
		return (print_err(INVALID_ARGS, 1));
	return (0);
}
