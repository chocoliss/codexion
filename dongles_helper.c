#include "codexion.h"

int	coder_case(t_sim *sim, int i, int left)
{
	dequeue(sim);
	sim->dongles[left].taken = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	print_state("has taken a dongle", &sim->coders[i]);
	return (0);
}

int	take(t_sim *sim, int i, int left, int right)
{
	if (sim->dongles[left].taken == 1)
		return (0);
	if (sim->dongles[right].taken == 1)
		return (0);
	if (sim->queue[sim->front] != i)
		return (0);
	if (get_time_ms() < sim->dongles[left].cooldown_until)
		return (0);
	if (get_time_ms() < sim->dongles[right].cooldown_until)
		return (0);
	return (1);
}

void	dongles_wait(t_sim *sim, int i, int left, int right)
{
	long			wake;
	struct timespec	ts;

	while (sim->stop == 0 && take(sim, i, left, right) == 0)
	{
		if (sim->dongles[left].taken == 0
			&& sim->dongles[right].taken == 0
			&& sim->queue[sim->front] == i)
		{
			wake = ft_max(sim->dongles[left].cooldown_until,
					sim->dongles[right].cooldown_until);
			ts = ms_to_timespec(wake);
			pthread_cond_timedwait(&sim->dongles_cond,
				&sim->state_mutex, &ts);
		}
		else
			pthread_cond_wait(&sim->dongles_cond, &sim->state_mutex);
	}
}

int	take_success(t_sim *sim, int i, int left, int right)
{
	dequeue(sim);
	sim->dongles[left].taken = 1;
	sim->dongles[right].taken = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	print_state("has taken a dongle", &sim->coders[i]);
	print_state("has taken a dongle", &sim->coders[i]);
	return (1);
}

long ft_max(long left,long right)
{
	if (left > right)
		return left;
	return right;
}
