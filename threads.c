#include "codexion.h"

void *coder_routine(void *coderi)
{
    t_coder *coder;
    
    coder = (t_coder *) coderi;
    print_state("started",coder);
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
    i = 0;
    while (i < sim->config.number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
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
    pthread_mutex_lock(&sim->log_mutex);
    printf("%ld %d %s\n",timestamp(sim), coder->id,msg);
    pthread_mutex_unlock(&sim->log_mutex);
}
