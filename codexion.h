#ifndef CODEXION_H
#define CODEXION_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdint.h> //in linux

typedef struct s_config
{
	int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	int	scheduler; // 0 = fifo, 1 = edf
}	t_config;

typedef struct s_sim t_sim;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	int			left_dongle;
	int			right_dongle;
	long		last_compile_start;
	int			compile_count;
	t_sim		*sim;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	int				taken;
	long			cooldown_until;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_sim
{
	t_config		config;
	long			start_time;
	int				stop;

	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	monitor;

	pthread_mutex_t	log_mutex;
	pthread_mutex_t	state_mutex;
}	t_sim;

void ft_write(void);
void	free_arguments(char **args);
int ft_strcmp(char *str1,char *str2);
long	ft_atoi(const char *str);
int	parsing(int ac, char **av, t_config *config);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int	check_av(char **arguments);
char	**normalize(int ac, char **av);
int	insert(char **arguments, t_config *config);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
int	ft_error(int ac, char **av);
int	finteger(char **arguments);
int ft_strlen(const char *str);

#endif