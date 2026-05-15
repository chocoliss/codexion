#include "codexion.h"

int	ft_error(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (av[i] == NULL || av[i][0] == '\0')
		{
			write(2, "Error\n", 6);
			return (1);
		}
		i++;
	}
	return (0);
}

int ft_strlen(const char *str)
{
    int i;
	
	i = 0;
    while(str[i])
        i++;
    return i;
}

int ft_strcmp(char *str1,char *str2)
{
	int i;

	i = 0;
	while ( str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return str[i] - str2[i]
}

int	finteger(char **arguments)
{
	int	i;
	int	j;

	i = 0;
	while (arguments[i] && i < 7)
	{
		j = 0;
		if (!arguments[i][0])
			return (1);
		if (arguments[i][j] == '+' || arguments[i][j] == '-')
			j++;
		if (!arguments[i][j])
			return (1);
		while (arguments[i][j])
		{
			if (arguments[i][j] < '0' || arguments[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}