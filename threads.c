/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imansar <imansar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 15:16:01 by imansar           #+#    #+#             */
/*   Updated: 2026/06/16 19:49:25 by imansar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (keep_compiling(sim, coder))
	{
		if (take_dongles(coder->id - 1, sim) == 0)
			return (NULL);
		print_state("is compiling", coder);
		increment_last_compile_start(sim, coder);
		if (smart_sleep(sim, sim->config.time_to_compile) == 1)
			return (release_dongles(coder->id - 1, sim), NULL);
		increment_compile_count(sim, coder);
		release_dongles(coder->id - 1, sim);
		print_state("is debugging", coder);
		if (smart_sleep(sim, sim->config.time_to_debug) == 1)
			return (NULL);
		print_state("is refactoring", coder);
		if (smart_sleep(sim, sim->config.time_to_refactor) == 1)
			return (NULL);
	}
	return (NULL);
}

int	create_threads(t_sim *sim)
{
	int	i;

	i = 0;
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return (1);
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
			return (stop_join(sim, i));
		i++;
	}
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (0);
}

int	start_simulation(t_sim *sim)
{
	int	i;

	i = 0;
	sim->start_time = get_time_ms();
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].last_compile_start = sim->start_time;
		i++;
	}
	if (create_threads(sim) == 1)
		return (1);
	return (0);
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

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			pthread_mutex_lock(&sim->state_mutex);
			if (get_time_ms()
				- sim->coders[i].last_compile_start >= 
				sim->config.time_to_burnout)
				return (sim->stop = 1, pthread_mutex_unlock(&sim->state_mutex),
					pthread_cond_broadcast(&sim->dongles_cond),
					print_burnout(&sim->coders[i]), NULL);
			pthread_mutex_unlock(&sim->state_mutex);
			i++;
		}
		if (all_coders_finished(sim) == 1)
			return (set_stop(sim), NULL);
		usleep(1000);
	}
	return (NULL);
}
