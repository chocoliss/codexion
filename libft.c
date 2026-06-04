#include "codexion.h"

int	ft_error(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (av[i] == NULL || av[i][0] == '\0') 
			return (1);
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
	return str1[i] - str2[i];
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

void ft_write(void)
{
	write(2, "make sure:\n",11);
	write(2, "1- number_of_coders (int)\n", 26);
	write(2, "2- time_to_burnout (int)\n", 25);
	write(2, "3- time_to_compile (int)\n", 25);
	write(2, "4- time_to_debug (int)\n", 23);
	write(2, "5- time_to_refactor (int)\n", 26);
	write(2, "6- number_of_compiles_required (int)\n", 37);
	write(2, "7- dongle_cooldown (int)\n", 25);
	write(2, "8- scheduler ('fifo' or 'edf')\n", 31);
}

void ft_putchar(int i)
{
    write(1,&i,1);
}

void ft_putnbr(int nb)
{
	if (nb == -2147483648)
	{
		write(1,"-2147483648",11);
		return;
	}
	if (nb < 0)
	{
		nb = -nb;
		write(1,"-",1);
	}
	if (nb > 9)
        ft_putnbr(nb / 10);
	ft_putchar((nb % 10) + 48);
	return;
}

void ft_putstr(char *str)
{
	while(*str)
		write(1,str++,1);
}