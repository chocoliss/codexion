/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:14:15 by imansar           #+#    #+#             */
/*   Updated: 2026/06/21 15:57:36 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clear_sim(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->state_mutex);
	pthread_cond_destroy(&sim->dongles_cond);
	free(sim->coders);
	free(sim->dongles);
	free(sim->queue);
	return ;
}

void	swap_queue(int *left, int *right)
{
	int	tmp;

	tmp = *left;
	*left = *right;
	*right = tmp;
}

int	main(int ac, char **av)
{
	t_config	config;
	t_sim		sim;

	if (parsing(ac, av, &config) == 1)
		return (1);
	if (init_sim(&config, &sim) != 0)
		return (1);
	if (start_simulation(&sim) != 0)
	{
		clear_sim(&sim);
		return (1);
	}
	clear_sim(&sim);
	return (0);
}
