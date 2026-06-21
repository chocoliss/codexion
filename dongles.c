/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:32 by imansar           #+#    #+#             */
/*   Updated: 2026/06/21 16:02:01 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongles(int i, t_sim *sim)
{
	int		left;
	int		right;
	long	cooldown;

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

void	increment_compile_count(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&sim->state_mutex);
}

void	increment_last_compile_start(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&sim->state_mutex);
}

int	keep_compiling(t_sim *sim, t_coder *coder)
{
	int	result;

	pthread_mutex_lock(&sim->state_mutex);
	result = (sim->stop == 0
			&& coder->compile_count < sim->config.number_of_compiles_required);
	pthread_mutex_unlock(&sim->state_mutex);
	return (result);
}

int	take_dongles(int i, t_sim *sim)
{
	int	left;
	int	right;

	pthread_mutex_lock(&sim->state_mutex);
	left = i;
	right = (i + 1) % sim->config.number_of_coders;
	if (sim->coders[i].compile_count >= sim->config.number_of_compiles_required)
		return (pthread_mutex_unlock(&sim->state_mutex), 0);
	enqueue(sim, i);
	if (sim->config.number_of_coders == 1)
		return (onecoder(sim, i, left));
	dongles_wait(sim, i, left, right);
	if (sim->stop == 1)
		return (dequeue_i(sim, i),
			pthread_mutex_unlock(&sim->state_mutex), 0);
	return (take_success(sim, i, left, right));
}
