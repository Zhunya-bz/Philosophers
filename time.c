#include "philo.h"

long	get_current_moment(void)
{
	struct timeval	t_start;

	gettimeofday(&t_start, NULL);
	return (t_start.tv_sec * 1000 + t_start.tv_usec / 1000);
}

int	get_time_passed(long starting_point)
{
	return ((int)(get_current_moment() - starting_point));
}

void	my_sleep(int ms)
{
	long	start;

	start = get_current_moment();
	while (get_current_moment() - start < ms)
		usleep(50);
}
