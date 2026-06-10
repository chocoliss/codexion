#include "codexion.h"

int *add_to_queue(int *queue, int coder_id,int index)
{
    queue[index + 1] = coder_id;
    return queue;
}

int *enqueue(t_sim *sim, int coder_id)
{
    int i;
    int *queue;

    queue = sim->queue;
    if (sim->config.scheduler == 0)
       return enqueue_fifo(sim,coder_id);
    else
        return enqueue_edf(sim, coder_id);
}

int *enqueue_fifo(t_sim *sim,int coder_id)
{
    int i;
    int *queue;

    queue = sim->queue;
    // queue = add_to_queue
    return queue;
}