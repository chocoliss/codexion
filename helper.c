#include "codexion.h"

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

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*s;

	if (dst == src)
		return (dst);
	s = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = s[i];
		i++;
	}
	return (dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	int		len1;
	int		len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	p = malloc(len1 + len2 + 1);
	if (!p)
		return (NULL);
	ft_memcpy(p, s1, len1);
	ft_memcpy(p + len1, s2, len2);
	p[len1 + len2] = '\0';
	return (p);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s);
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, len + 1);
	return (dup);
}
