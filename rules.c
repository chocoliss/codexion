/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:15:55 by imansar           #+#    #+#             */
/*   Updated: 2026/06/16 19:47:10 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_arguments(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	print_burnout(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->log_mutex);
	ft_putnbr((int)timestamp(sim));
	write(1, " ", 1);
	ft_putnbr(coder->id);
	write(1, " ", 1);
	ft_putstr("burned out\n");
	write(1, "\n", 1);
	pthread_mutex_unlock(&sim->log_mutex);
}

int	all_coders_finished(t_sim *sim)
{
	int	i;

	if (sim->config.number_of_compiles_required <= 0)
		return (0);
	i = 0;
	pthread_mutex_lock(&sim->state_mutex);
	while (i < sim->config.number_of_coders)
	{
		if (sim->config.number_of_compiles_required > 
			sim->coders[i].compile_count)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&sim->state_mutex);
	return (1);
}

void	dequeue_i(t_sim *sim, int coder_id)
{
	int	j;
	int	n;

	n = sim->config.number_of_coders;
	j = 0;
	while (j < sim->count)
	{
		if (sim->queue[(sim->front + j) % n] == coder_id)
		{
			while (j < sim->count - 1)
			{
				sim->queue[(sim->front + j) % n] = sim->queue[(sim->front + j
						+ 1) % n];
				j++;
			}
			sim->rear = (sim->rear - 1 + n) % n;
			sim->count--;
			return ;
		}
		j++;
	}
}

void	ft_write(void)
{
	write(2, "make sure:\n", 11);
	write(2, "1- number_of_coders (positive int)\n", 35);
	write(2, "2- time_to_burnout (int)\n", 25);
	write(2, "3- time_to_compile (int)\n", 25);
	write(2, "4- time_to_debug (int)\n", 23);
	write(2, "5- time_to_refactor (int)\n", 26);
	write(2, "6- number_of_compiles_required (positive int)\n", 46);
	write(2, "7- dongle_cooldown (int)\n", 25);
	write(2, "8- scheduler ('fifo' or 'edf')\n", 31);
}
