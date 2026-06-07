#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	timestamp(t_sim *sim)
{
	return (get_time_ms() - sim->start_time);
}

void set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stop = 1;
	pthread_cond_broadcast(&sim->dongles_cond);
	pthread_mutex_unlock(&sim->state_mutex);
}

struct timespec ms_to_timespec(long ms)
{
    struct timespec ts;

    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    return (ts);
}

int is_stopped(t_sim *sim)
{
	int stop;

	pthread_mutex_lock(&sim->state_mutex);
	stop = sim->stop ;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stop);
}

int	smart_sleep(t_sim *sim,long duration_ms)
{
	long start;

	start = get_time_ms();
	while (get_time_ms() - start < duration_ms)
	{
		if (is_stopped(sim) == 1)
			return 1;
		usleep(1000);
	}
	return 0;
}
