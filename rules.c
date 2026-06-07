#include "codexion.h"

void	free_arguments(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void print_burnout(t_coder *coder)
{
    t_sim *sim;

    sim = coder->sim;
    pthread_mutex_lock(&sim->log_mutex);
    ft_putnbr((int)timestamp(sim));
    write(1," ",1);
    ft_putnbr(coder->id);
    write(1," ",1);
    ft_putstr("burned out\n");
    write(1,"\n",1);	
    pthread_mutex_unlock(&sim->log_mutex);
}

int all_coders_finished(t_sim *sim)
{
	int i;

	if (sim->config.number_of_compiles_required <= 0)
		return (0);
	
	i = 0;
	pthread_mutex_lock(&sim->state_mutex);
	while (i < sim->config.number_of_coders)
	{
		if (sim->config.number_of_compiles_required > sim->coders[i].compile_count)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			return(0);
		}
		i++;
	}
	pthread_mutex_unlock(&sim->state_mutex);
	return (1);
}