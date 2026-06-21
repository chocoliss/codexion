/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:15:55 by imansar           #+#    #+#             */
/*   Updated: 2026/06/21 15:54:33 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	ft_error(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (av[i] == NULL || av[i][0] == '\0')
			return (1);
		i++;
	}
	return (0);
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
	ft_putstr("burned out");
	write(1, "\n", 1);
	pthread_mutex_unlock(&sim->log_mutex);
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
