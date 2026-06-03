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
    printf("%ld %d burned out\n", timestamp(sim), coder->id);
    pthread_mutex_unlock(&sim->log_mutex);
}

int all_coders_finished(t_sim *sim)
{
	int i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->config.number_of_compiles_required > sim->coders[i].compile_count)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			return(0);
		}
		pthread_mutex_unlock(&sim->state_mutex);
		i++;
	}
	return 1;
}