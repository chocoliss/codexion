/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:42 by imansar           #+#    #+#             */
/*   Updated: 2026/06/14 15:14:43 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	ft_putchar(int i)
{
	write(1, &i, 1);
}

void	ft_putnbr(int nb)
{
	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	if (nb < 0)
	{
		nb = -nb;
		write(1, "-", 1);
	}
	if (nb > 9)
		ft_putnbr(nb / 10);
	ft_putchar((nb % 10) + 48);
	return ;
}

void	ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
}
