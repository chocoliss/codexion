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
	i = 0;
	while (i < created)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (1);
}

