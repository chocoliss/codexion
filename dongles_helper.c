/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:45:56 by imansar           #+#    #+#             */
/*   Updated: 2026/06/21 15:58:59 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	onecoder(t_sim *sim, int i, int left)
{
	dequeue_i(sim, i);
	sim->dongles[left].taken = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	print_state("has taken a dongle", &sim->coders[i]);
	return (0);
}

int	take(t_sim *sim, int i, int left, int right)
{
	if (priority(sim, i) == 0)
		return (0);
	if (coder_can_take(sim, left, right) == 0)
		return (0);
	return (1);
}

void	dongles_wait(t_sim *sim, int i, int left, int right)
{
	long			wake;
	long			now;
	struct timespec	ts;

	while (sim->stop == 0 && take(sim, i, left, right) == 0)
	{
		now = get_time_ms();
		wake = ft_max(sim->dongles[left].cooldown_until,
				sim->dongles[right].cooldown_until);
		if (sim->dongles[left].taken == 0 && sim->dongles[right].taken == 0
			&& wake > now)
		{
			ts = ms_to_timespec(wake);
			pthread_cond_timedwait(&sim->dongles_cond, &sim->state_mutex, &ts);
		}
		else
			pthread_cond_wait(&sim->dongles_cond, &sim->state_mutex);
	}
}

int	take_success(t_sim *sim, int i, int left, int right)
{
	dequeue_i(sim, i);
	sim->dongles[left].taken = 1;
	sim->dongles[right].taken = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	print_state("has taken a dongle", &sim->coders[i]);
	print_state("has taken a dongle", &sim->coders[i]);
	return (1);
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
		if (other != i && share_dongle(sim, other, i) && queue_less(sim, other,
				i) && coder_can_take(sim, other, (other + 1)
				% sim->config.number_of_coders))
			return (0);
		pos++;
	}
	return (1);
}
