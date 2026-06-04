#include "codexion.h"

void *coder_routine(void *arg)
{
    t_coder *coder;
    t_sim *sim;
    
    coder = (t_coder *) arg;
    sim = coder->sim;
    while(!is_stopped(sim) || coder->compile_count >= sim->config.number_of_compiles_required )
    {
        if (take_dongles(coder->id - 1, sim) == 0)
            return (NULL);
        
        print_state("is compiling",coder);
        pthread_mutex_lock(&sim->state_mutex);
        coder->last_compile_start = get_time_ms();
        pthread_mutex_unlock(&sim->state_mutex);
        
        if (smart_sleep(sim, sim->config.time_to_compile) == 1)
            return (release_dongles(coder->id - 1,sim),NULL);
        release_dongles(coder->id - 1,sim);

        pthread_mutex_lock(&sim->state_mutex);
        coder->compile_count += 1;
        pthread_mutex_unlock(&sim->state_mutex);

        print_state("is debugging",coder);
        if (smart_sleep(sim, sim->config.time_to_debug) == 1)
        return (NULL);
        
        print_state("is refactoring",coder);
        if (smart_sleep(sim, sim->config.time_to_refactor) == 1)
            return NULL;
    }
    return NULL;
}

int create_threads(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        if (pthread_create(&sim->coders[i].thread,NULL,coder_routine,&sim->coders[i]) != 0 )
        return 1;
        i++;
    }
    if (pthread_create(&sim->monitor, NULL, monitor_routine,sim) != 0)
        return 1;
    i = 0;
    while (i < sim->config.number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
    pthread_join(sim->monitor,NULL);
    return 0;
}

int start_simulation(t_sim *sim)
{
    int i;

    i = 0;
    sim->start_time = get_time_ms();
    while (i < sim->config.number_of_coders)
    {
        sim->coders[i].last_compile_start = sim->start_time;
        i++;
    }
    if (create_threads(sim) == 1)
        return 1;
    return 0;
}

void print_state(char *msg, t_coder *coder)
{
    t_sim *sim;

    sim = coder->sim;
    if (is_stopped(sim) == 1)
        return ;
    pthread_mutex_lock(&sim->log_mutex);
    ft_putnbr((int)timestamp(sim));
    write(1," ",1);
    ft_putnbr(coder->id);
    write(1," ",1);
    ft_putstr(msg);
    write(1,"\n",1);
    pthread_mutex_unlock(&sim->log_mutex);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			pthread_mutex_lock(&sim->state_mutex);
			if (get_time_ms() - sim->coders[i].last_compile_start
				>= sim->config.time_to_burnout)
			{
				sim->stop = 1;
				pthread_mutex_unlock(&sim->state_mutex);
				return (print_burnout(&sim->coders[i]), NULL);
			}
			pthread_mutex_unlock(&sim->state_mutex);
            i++;
		}
        if (all_coders_finished(sim) == 1)
            return (set_stop(sim),NULL);
		usleep(1000);
	}
	return (NULL);
}
