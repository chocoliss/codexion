/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:15:59 by imansar           #+#    #+#             */
/*   Updated: 2026/06/14 15:55:36 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	alloc_sim_arrays(t_config *config, t_sim *sim)
{
	sim->queue = malloc(sizeof(int) * config->number_of_coders);
	sim->queued = malloc(sizeof(int) * config->number_of_coders);
	sim->queue_order = malloc(sizeof(long) * config->number_of_coders);
	sim->dongles = malloc(sizeof(t_dongle) * config->number_of_coders);
	sim->coders = malloc(sizeof(t_coder) * config->number_of_coders);
	if (!sim->queue || !sim->queued || !sim->queue_order
		|| !sim->dongles || !sim->coders)
		return (1);
	memset(sim->queued, 0, sizeof(int) * config->number_of_coders);
	memset(sim->queue_order, 0, sizeof(long) * config->number_of_coders);
	memset(sim->dongles, 0, sizeof(t_dongle) * config->number_of_coders);
	memset(sim->coders, 0, sizeof(t_coder) * config->number_of_coders);
	return (0);
}

int	init_sim(t_config *config, t_sim *sim)
{
	memset(sim, 0, sizeof(t_sim));
	sim->config = *config;
	if (alloc_sim_arrays(config, sim) == 1)
		return (free(sim->queue), free(sim->queued), free(sim->queue_order),
			free(sim->dongles), free(sim->coders), 1);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (free(sim->queue), free(sim->queued), free(sim->queue_order),
			free(sim->dongles), free(sim->coders), 1);
	sim->log_mutex_ready = 1;
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (clear_sim(sim), 1);
	sim->state_mutex_ready = 1;
	if (pthread_cond_init(&sim->dongles_cond, NULL) != 0)
		return (clear_sim(sim), 1);
	sim->dongles_cond_ready = 1;
	fill_coders(config, sim);
	if (fill_dongles(config, sim) == 1)
		return (clear_sim(sim), 1);
	return (0);
}

void	fill_coders(t_config *config, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].left_dongle = i;
		if (i + 1 == config->number_of_coders)
			sim->coders[i].right_dongle = 0;
		else
			sim->coders[i].right_dongle = i + 1;
		sim->coders[i].last_compile_start = get_time_ms();
		sim->coders[i].sim = sim;
		i++;
	}
}

int	fill_dongles(t_config *config, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].taken = 0;
		sim->dongles[i].cooldown_until = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (pthread_mutex_destroy(&sim->dongles[i].mutex), 1);
		sim->dongles_ready++;
		i++;
	}
	return (0);
}

int	is_stopped(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->state_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stop);
}
