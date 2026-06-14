/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:36 by imansar           #+#    #+#             */
/*   Updated: 2026/06/14 15:14:37 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	count_length_of_words(char *str, char c, int pos)
{
	int	i;

	i = 0;
	while (str[pos + i] != c && str[pos + i])
		i++;
	return (i);
}

static int	count_words(char *str, char c)
{
	int	i;
	int	w;

	i = 0;
	w = 0;
	while (str[i])
	{
		if (str[i] != c)
		{
			w++;
			while (str[i] && str[i] != c)
				i++;
		}
		else
			i++;
	}
	return (w);
}

static void	*free_mem(char **tab, int j)
{
	int	e;

	e = 0;
	while (e < j)
	{
		free(tab[e]);
		e++;
	}
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i[3];

	i[0] = 0;
	i[1] = 0;
	if (!s)
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * (count_words((char *)s, c) + 1));
	if (!tab)
		return (NULL);
	while (s[i[0]])
	{
		if (s[i[0]] != c)
		{
			i[2] = count_length_of_words((char *)s, c, i[0]);
			tab[i[1]] = ft_substr(s, i[0], i[2]);
			if (!tab[i[1]])
				return (free_mem(tab, i[1]));
			i[1]++;
			i[0] += i[2];
		}
		else
			i[0]++;
	}
	return (tab[i[1]] = NULL, tab);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*p;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
	{
		p = (char *)malloc(1);
		if (!p)
			return (NULL);
		p[0] = '\0';
		return (p);
	}
	if (len > slen - start)
		len = slen - start;
	p = (char *)malloc((len + 1));
	if (!p)
		return (NULL);
	ft_memcpy(p, s + start, len);
	p[len] = '\0';
	return (p);
}