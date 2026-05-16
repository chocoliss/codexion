#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <unistd.h>

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

int main()
{
    printf("tes	:%ld\n",ft_atoi("\0"));
}