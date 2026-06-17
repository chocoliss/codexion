/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:58 by imansar           #+#    #+#             */
/*   Updated: 2026/06/14 15:14:59 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_av(char **arguments)
{
	int		i;
	int		k;
	long	j;
	long	g;

	if (finteger(arguments) == 1 || !arguments)
		return (1);
	i = 0;
	while (arguments[i] && i < 7)
	{
		j = ft_atoi(arguments[i]);
		if (j == LONG_MAX || j == LONG_MIN)
			return (1);
		k = i + 1;
		while (arguments[k] && k < 7)
		{
			g = ft_atoi(arguments[k]);
			if (g == LONG_MAX || g == LONG_MIN)
				return (1);
			k++;
		}
		i++;
	}
	return (0);
}

static char	*build_string(int ac, char **av)
{
	char	*s;
	char	*t;
	int		i;

	s = ft_strdup(av[1]);
	if (!s)
		return (NULL);
	i = 2;
	while (i < ac)
	{
		t = s;
		s = ft_strjoin(t, " ");
		free(t);
		if (!s)
			return (NULL);
		t = s;
		s = ft_strjoin(t, av[i++]);
		free(t);
		if (!s)
			return (NULL);
	}
	return (s);
}

char	**normalize(int ac, char **av)
{
	char	**arg;
	char	*s;

	if (!av)
		return (NULL);
	s = build_string(ac, av);
	if (!s)
		return (NULL);
	arg = ft_split(s, ' ');
	free(s);
	return (arg);
}

int	insert(char **arguments, t_config *config)
{
	if (ft_atoi(arguments[0]) <= 0)
		return (1);
	config->number_of_coders = ft_atoi(arguments[0]);
	config->time_to_burnout = ft_atoi(arguments[1]);
	config->time_to_compile = ft_atoi(arguments[2]);
	config->time_to_debug = ft_atoi(arguments[3]);
	config->time_to_refactor = ft_atoi(arguments[4]);
	if (ft_atoi(arguments[5]) <= 0)
		return (1);
	config->number_of_compiles_required = ft_atoi(arguments[5]);
	config->dongle_cooldown = ft_atoi(arguments[6]);
	if (strcmp("fifo", arguments[7]) == 0)
		config->scheduler = 0;
	else if (strcmp("edf", arguments[7]) == 0)
		config->scheduler = 1;
	else
		return (1);
	return (0);
}

int	parsing(int ac, char **av, t_config *config)
{
	char	**arguments;

	if (ac != 9 || ft_error(ac, av) == 1)
		return (ft_write(), 1);
	arguments = normalize(ac, av);
	if (!arguments || check_av(arguments) == 1 || insert(arguments,
			config) == 1)
		return (free_arguments(arguments), ft_write(), 1);
	free_arguments(arguments);
	return (0);
}
