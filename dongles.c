#include "codexion.h"

// int dongles(t_sim *sim)
// {
//     int i;

//     i = 0;
    
//     while(i < sim->config.number_of_coders && take_dongles(i,sim) == 0)
//     {
//         pthread_mutex_lock(&sim->dongles[i].mutex);

//         pthread_mutex_unlock(&sim->dongles[i].mutex);
//         i++;
//     }
// }

int take_dongles(int i,t_sim *sim)
{
    int succes;
    int left;
    int right;

    succes = 0;
    pthread_mutex_lock(&sim->state_mutex);
    left = i;
    right = (i + 1) % sim->config.number_of_coders;
    while ( sim->stop == 0 &&(sim->dongles[left].taken == 1 || sim->dongles[right].taken == 1 ))
    {
        pthread_cond_wait(&sim->dongles_cond,&sim->state_mutex);
    }
    if (sim->stop == 0)
    {
        sim->dongles[left].taken = 1;
        sim->dongles[right].taken = 1;
        succes = 1;
    }
    pthread_mutex_unlock(&sim->state_mutex);
    return (succes);
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
