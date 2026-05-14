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
	while (arguments[i])
	{
		j = ft_atoi(arguments[i]);
		if (j == LONG_MAX || j == LONG_MIN)
			return (1);
		k = i + 1;
		while (arguments[k])
		{
			g = ft_atoi(arguments[k]);
			if (g == j || g == LONG_MAX || g == LONG_MIN)
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

int	*insert(char **arguments)
{
	int		i;

	int *array = malloc(8 * sizeof(int));
	if (!array)
		return NULL;
	i = 0;
	while (arguments[i])
	{
		array[i] = ft_atoi(arguments[i]);
		i++;
	}
	return (array);
}

int	parsing(int ac, char **av)
{
	char	**arguments;
	int *input;

	if (ac < 8 || ft_error(ac, av) == 1)
		return (1);
	arguments = normalize(ac, av);
	if (!arguments || check_av(arguments) == 1)
		return (write(2, "Error\n", 6), 1);
	input = insert(arguments);
    for(int i = 0; i < 8; i++)
        printf("arguments[%d] = %d", i, input[i]);
	return (0);
}
