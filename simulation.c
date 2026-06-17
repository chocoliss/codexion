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

int	init_sim(t_config *config, t_sim *sim)
{
	sim->config = *config;
	sim->start_time = 0;
	sim->stop = 0;
	sim->queue = (int *)malloc(sizeof(int) * config->number_of_coders);
	sim->front = 0;
	sim->rear = 0;
	sim->count = 0;
	sim->dongles = malloc(sizeof(t_dongle) * config->number_of_coders);
	if (!sim->dongles)
		return (1);
	sim->coders = malloc(sizeof(t_coder) * config->number_of_coders);
	if (!sim->coders)
	{
		free(sim->dongles);
		return (1);
	}
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->state_mutex, NULL);
	fill_coders(config, sim);
	fill_dongles(config, sim);
	pthread_cond_init(&sim->dongles_cond, NULL);
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
	return ;
}

void	fill_dongles(t_config *config, t_sim *sim)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].taken = 0;
		sim->dongles[i].cooldown_until = 0;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		i++;
	}
	return ;
}

int	is_stopped(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->state_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stop);
}
