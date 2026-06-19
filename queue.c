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

long	get_deadline(t_sim *sim, int coder_id)
{
	return (sim->coders[coder_id].last_compile_start
		+ sim->config.time_to_burnout);
}

// void	enqueue(t_sim *sim, int coder_id)
// {
// 	if (sim->config.scheduler == 0)
// 		enqueue_fifo(sim, coder_id);
// 	else
// 		enqueue_edf(sim, coder_id);
// }

// void	enqueue_fifo(t_sim *sim, int coder_id)
// {
// 	sim->queue[sim->rear] = coder_id;
// 	sim->rear = (sim->rear + 1) % sim->config.number_of_coders;
// 	sim->count++;
// 	return ;
// }

// int	dequeue(t_sim *sim)
// {
// 	int	i;

// 	if (sim->count == 0)
// 		return (0);
// 	i = sim->queue[sim->front];
// 	sim->front = (sim->front + 1) % sim->config.number_of_coders;
// 	sim->count--;
// 	return (i);
// }

// void	enqueue_edf(t_sim *sim, int coder_id)
// {
// 	long	deadline;
// 	int		i;
// 	int		j;
// 	int		n;

// 	n = sim->config.number_of_coders;
// 	deadline = get_deadline(sim, coder_id);
// 	i = 0;
// 	while (i < sim->count)
// 	{
// 		if (deadline < get_deadline(sim, sim->queue[(sim->front + i) % n]))
// 			break ;
// 		i++;
// 	}
// 	j = sim->count;
// 	while (j > i)
// 	{
// 		sim->queue[(sim->front + j) % n] = sim->queue[(sim->front + j - 1) % n];
// 		j--;
// 	}
// 	sim->queue[(sim->front + i) % n] = coder_id;
// 	sim->rear = (sim->rear + 1) % n;
// 	sim->count++;
// }


static int	queue_less(t_sim *sim, int left, int right)
{
	long	left_deadline;
	long	right_deadline;

	if (sim->config.scheduler == 1)
	{
		left_deadline = get_deadline(sim, left);
		right_deadline = get_deadline(sim, right);
		if (left_deadline != right_deadline)
			return (left_deadline < right_deadline);
	}
	if (sim->queue_order[left] != sim->queue_order[right])
		return (sim->queue_order[left] < sim->queue_order[right]);
	return (left < right);
}

static void	swap_queue(int *left, int *right)
{
	int	tmp;

	tmp = *left;
	*left = *right;
	*right = tmp;
}

static void	heap_up(t_sim *sim, int pos)
{
	int	parent;

	while (pos > 0)
	{
		parent = (pos - 1) / 2;
		if (!queue_less(sim, sim->queue[pos], sim->queue[parent]))
			return ;
		swap_queue(&sim->queue[pos], &sim->queue[parent]);
		pos = parent;
	}
}

static void	heap_down(t_sim *sim, int pos)
{
	int	left;
	int	right;
	int	best;

	while (1)
	{
		left = (pos * 2) + 1;
		right = (pos * 2) + 2;
		best = pos;
		if (left < sim->count
			&& queue_less(sim, sim->queue[left], sim->queue[best]))
			best = left;
		if (right < sim->count
			&& queue_less(sim, sim->queue[right], sim->queue[best]))
			best = right;
		if (best == pos)
			return ;
		swap_queue(&sim->queue[pos], &sim->queue[best]);
		pos = best;
	}
}

void	enqueue(t_sim *sim, int coder_id)
{
	if (sim->queued[coder_id] == 1)
		return ;
	if (sim->count >= sim->config.number_of_coders)
		return ;
	sim->queued[coder_id] = 1;
	sim->queue_order[coder_id] = sim->next_order++;
	sim->queue[sim->count] = coder_id;
	heap_up(sim, sim->count);
	sim->count++;
}

int	dequeue(t_sim *sim)
{
	int	coder_id;

	if (sim->count == 0)
		return (-1);
	coder_id = sim->queue[0];
	sim->queued[coder_id] = 0;
	sim->count--;
	if (sim->count > 0)
	{
		sim->queue[0] = sim->queue[sim->count];
		heap_down(sim, 0);
	}
	return (coder_id);
}

void	dequeue_i(t_sim *sim, int coder_id)
{
	int	pos;

	if (coder_id < 0 || coder_id >= sim->config.number_of_coders)
		return ;
	if (sim->queued[coder_id] == 0)
		return ;
	pos = 0;
	while (pos < sim->count && sim->queue[pos] != coder_id)
		pos++;
	if (pos == sim->count)
		return ;
	sim->queued[coder_id] = 0;
	sim->count--;
	if (pos < sim->count)
	{
		sim->queue[pos] = sim->queue[sim->count];
		heap_up(sim, pos);
		heap_down(sim, pos);
	}
}

static int	share_dongle(t_sim *sim, int left_coder, int right_coder)
{
	int	left_a;
	int	right_a;
	int	left_b;
	int	right_b;
	int	n;

	n = sim->config.number_of_coders;
	left_a = left_coder;
	right_a = (left_coder + 1) % n;
	left_b = right_coder;
	right_b = (right_coder + 1) % n;
	return (left_a == left_b || left_a == right_b
		|| right_a == left_b || right_a == right_b);
}

int	priority(t_sim *sim, int i)
{
	int	pos;
	int	other;

	if (sim->queued[i] == 0)
		return (0);
	pos = 0;
	while (pos < sim->count)
	{
		other = sim->queue[pos];
		if (other != i && share_dongle(sim, other, i)
			&& queue_less(sim, other, i)
			&& coder_can_take(sim, other, (other + 1)
				% sim->config.number_of_coders))
			return (0);
		pos++;
	}
	return (1);
}
