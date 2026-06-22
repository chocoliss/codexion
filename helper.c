/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 15:26:56 by imansar           #+#    #+#             */
/*   Updated: 2026/06/21 15:47:23 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coder_can_take(t_sim *sim, int left, int right)
{
	long	now;

	now = get_time_ms();
	if (sim->dongles[left].taken == 1)
		return (0);
	if (sim->dongles[right].taken == 1)
		return (0);
	if (now < sim->dongles[left].cooldown_until)
		return (0);
	if (now < sim->dongles[right].cooldown_until)
		return (0);
	return (1);
}

int	stop_join(t_sim *sim, int created)
{
	int	i;

	set_stop(sim);
	pthread_cond_broadcast(&sim->dongles_cond);
	i = 0;
	while (i < created)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (1);
}

void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->state_mutex);
}

void	print_state(char *msg, t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (is_stopped(sim) == 1)
		return ;
	pthread_mutex_lock(&sim->log_mutex);
	ft_putnbr((int)timestamp(sim));
	write(1, " ", 1);
	ft_putnbr(coder->id);
	write(1, " ", 1);
	ft_putstr(msg);
	write(1, "\n", 1);
	pthread_mutex_unlock(&sim->log_mutex);
}

long	ft_max(long left, long right)
{
	if (left > right)
		return (left);
	return (right);
}
