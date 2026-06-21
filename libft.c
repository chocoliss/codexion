/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:42 by imansar           #+#    #+#             */
/*   Updated: 2026/06/21 15:38:02 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

long	ft_atoi(const char *str)
{
	long	value;
	long	sign;
	int		digit;

	value = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		digit = (*str - '0');
		value = value * 10 + digit;
		if (sign == 1 && value > INT32_MAX)
			return (LONG_MAX);
		if (sign == -1 && value > 2147483648L)
			return (LONG_MIN);
		str++;
	}
	return ((value * sign));
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
