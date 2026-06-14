/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:32 by imansar           #+#    #+#             */
/*   Updated: 2026/06/14 15:18:55 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int take_dongles(int i,t_sim *sim)
{
    int left;
    int right;
    int index;
    struct timespec ts;

    pthread_mutex_lock(&sim->state_mutex);
    if (sim->coders[i].compile_count >= sim->config.number_of_compiles_required)
        return (pthread_mutex_unlock(&sim->state_mutex), 0);
    enqueue(sim, i);
    left = i;
    right = (i + 1) % sim->config.number_of_coders;
    if (left == right)
        return(dequeue(sim), sim->dongles[left].taken = 1, pthread_mutex_unlock(&sim->state_mutex),print_state("has taken a dongle", &sim->coders[i]),0);
    while (sim->stop == 0 && (sim->dongles[left].taken == 1 || sim->dongles[right].taken == 1 || sim->queue[sim->front] != i ||
        get_time_ms() < sim->dongles[right].cooldown_until || get_time_ms() < sim->dongles[left].cooldown_until))
    {
        if (!sim->dongles[left].taken && !sim->dongles[right].taken && sim->queue[sim->front] == i)
        {
            long wake = ft_max(sim->dongles[left].cooldown_until, sim->dongles[right].cooldown_until);
            ts = ms_to_timespec(wake);
            pthread_cond_timedwait(&sim->dongles_cond, &sim->state_mutex, &ts);
        }
        else
            pthread_cond_wait(&sim->dongles_cond, &sim->state_mutex);
    }
    if (sim->stop == 1)
        return (dequeue_i(sim,i), pthread_mutex_unlock(&sim->state_mutex), 0);
    dequeue(sim);
    sim->dongles[left].taken = 1;
    sim->dongles[right].taken = 1;
    return(pthread_mutex_unlock(&sim->state_mutex), print_state("has taken a dongle", &sim->coders[i]), print_state("has taken a dongle", &sim->coders[i]), 1);
}

void    release_dongles(int i, t_sim *sim)
{
    int left;
    int right;
    long cooldown;

    left = i;
    right = (i + 1) % sim->config.number_of_coders;
    pthread_mutex_lock(&sim->state_mutex);
    if (sim->dongles[left].taken == 1 && sim->dongles[right].taken == 1)
    {
        cooldown = get_time_ms() + sim->config.dongle_cooldown;
        sim->dongles[left].taken = 0;
        sim->dongles[right].taken = 0;
        sim->dongles[left].cooldown_until = cooldown;
        sim->dongles[right].cooldown_until = cooldown;
        pthread_cond_broadcast(&sim->dongles_cond);
    }
    pthread_mutex_unlock(&sim->state_mutex);
}

void increment_compile_count(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->state_mutex);
    coder->compile_count++;
    pthread_mutex_unlock(&sim->state_mutex);
}

void increment_last_compile_start(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->state_mutex);
    coder->last_compile_start = get_time_ms();
    pthread_mutex_unlock(&sim->state_mutex);
}

int keep_compiling(t_sim *sim, t_coder *coder)
{
    int result;

    pthread_mutex_lock(&sim->state_mutex);
    result = (sim->stop == 0 &&
              coder->compile_count < sim->config.number_of_compiles_required);
    pthread_mutex_unlock(&sim->state_mutex);
    return (result);
}