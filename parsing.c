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
	while (arguments[i] && i < 8)
	{
		j = ft_atoi(arguments[i]);
		if (j == LONG_MAX || j == LONG_MIN)
			return (1);
		k = i + 1;
		while (arguments[k] && k < 8)
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

t_config	*insert(char **arguments, t_config *config)
{
	config->number_of_coders = arguments[0]
	config->time_to_burnout = arguments
	config->time_to_compile = arguments
	config->time_to_debug = arguments
	config->time_to_refactor = arguments
	config->number_of_compiles_required = arguments
	config->dongle_cooldown = arguments
	if (ft_strcmp("fifo",arguments[8]))
		config->scheduler = 0
	else
		config->scheduler = 1
	return (config);
}

int	parsing(int ac, char **av, t_config *config)
{
	char	**arguments;
	int *input;

	if (ac != 9 || ft_error(ac, av) == 1 || (ft_strcmp(av[8],"fifo") != 0 || ft_strcmp(av[8],"edf"))) 
		return (write(2, "Error\n", 6), 1);
	arguments = normalize(ac - 1, av);
	if (!arguments || check_av(arguments) == 1)
		return (write(2, "Error\n", 6), 1);
	config = insert(arguments,config);
	return (0);
}
