#include "codexion.h"

int take_dongles(int i,t_sim *sim)
{
    int left;
    int right;

    pthread_mutex_lock(&sim->state_mutex);
    left = i;
    right = (i + 1) % sim->config.number_of_coders;
    if (sim->coders[i].compile_count >= sim->config.number_of_compiles_required)
        return (pthread_mutex_unlock(&sim->state_mutex), 0);
    if (left == right)
    {
        sim->dongles[left].taken = 1;
        return(pthread_mutex_unlock(&sim->state_mutex),print_state("has taken a dongle", &sim->coders[i]),0);
    }
    while (sim->stop == 0 && (sim->dongles[left].taken == 1 || sim->dongles[right].taken == 1 ))
    {
        pthread_cond_wait(&sim->dongles_cond,&sim->state_mutex);
    }
    if (sim->stop == 0)
    {
        sim->dongles[left].taken = 1;
        sim->dongles[right].taken = 1;
        pthread_mutex_unlock(&sim->state_mutex);
        return(print_state("has taken a dongle", &sim->coders[i]), print_state("has taken a dongle", &sim->coders[i]), 1);
    }
    return (pthread_mutex_unlock(&sim->state_mutex), 0);
}

void    release_dongles(int i, t_sim *sim)
{
    int left;
    int right;

    left = i;
    right = (i + 1) % sim->config.number_of_coders;
    pthread_mutex_lock(&sim->state_mutex);
    if (sim->dongles[left].taken == 1 && sim->dongles[right].taken == 1)
    {
        sim->dongles[left].taken = 0;
        sim->dongles[right].taken = 0;
        pthread_cond_broadcast(&sim->dongles_cond);
    }
    pthread_mutex_unlock(&sim->state_mutex);
}
