#ifndef CODEXION_H
#define CODEXION_H

# include <limits.h>
# include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

long	ft_atoi(const char *str);
int	parsing(int ac, char **av);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int	check_av(char **arguments);
char	**normalize(int ac, char **av);
int	*insert(char **arguments);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
int	ft_error(int ac, char **av);
int	finteger(char **arguments);
int ft_strlen(const char *str);

#endif