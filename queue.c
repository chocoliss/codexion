/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:15:49 by imansar           #+#    #+#             */
/*   Updated: 2026/06/14 15:30:20 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long get_deadline(t_sim *sim, int coder_id)
{
    return (sim->coders[coder_id].last_compile_start
        + sim->config.time_to_burnout);
}

void enqueue(t_sim *sim, int coder_id)
{
    if (sim->config.scheduler == 0)
        enqueue_fifo(sim,coder_id);
    else
        enqueue_edf(sim, coder_id);
}

void enqueue_fifo(t_sim *sim,int coder_id)
{
    sim->queue[sim->rear] = coder_id;
    sim->rear = (sim->rear + 1) % sim->config.number_of_coders;
    sim->count++;
    return;
}

int dequeue(t_sim *sim)
{
    int i;

    if (sim->count == 0)
        return (0);
    i = sim->queue[sim->front];
    sim->front = (sim->front + 1) % sim->config.number_of_coders;
    sim->count--;
    return i;
}

void enqueue_edf(t_sim *sim, int coder_id)
{
    long    deadline;
    int     i;
    int     j;
    int     n;

    n = sim->config.number_of_coders;
    deadline = get_deadline(sim, coder_id);
    i = 0;
    while (i < sim->count)
    {
        if (deadline < get_deadline(sim, sim->queue[(sim->front + i) % n]))
            break;
        i++;
    }
    j = sim->count;
    while (j > i)
    {
        sim->queue[(sim->front + j) % n] = sim->queue[(sim->front + j - 1) % n];
        j--;
    }
    sim->queue[(sim->front + i) % n] = coder_id;
    sim->rear = (sim->rear + 1) % n;
    sim->count++;
}
