/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:15:49 by imansar           #+#    #+#             */
/*   Updated: 2026/06/21 15:56:55 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	queue_less(t_sim *sim, int left, int right)
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

void	heap_up(t_sim *sim, int pos)
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

void	heap_down(t_sim *sim, int pos)
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

int	share_dongle(t_sim *sim, int left_coder, int right_coder)
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
