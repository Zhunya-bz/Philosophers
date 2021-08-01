#include "philo.h"

static int	die_2(t_philo *philo, t_data *data, int i)
{
	if (data->time_die + 1 <= get_time_passed(data->time_start)
		- philo[i].last_eat)
	{
		data->dead = 1;
		print_on_screen(&philo[i], 0);
		return (1);
	}
	if (philo[i].count_eat == data->nbr_eat)
	{
		data->eat_all++;
		if (data->eat_all == data->nbr_ph)
		{
			printf(YELLOW"%d: all the philosophers ate\n",
				get_time_passed(data->time_start));
			return (1);
		}
	}
	return (0);
}

void	*die(void *tmp)
{
	t_philo	*philo;
	t_data	*data;
	int		i;

	philo = tmp;
	data = philo[0].next_d;
	while (1)
	{
		data->eat_all = 0;
		i = 0;
		while (i < data->nbr_ph)
		{
			if (die_2(philo, data, i))
				return (NULL);
			i++;
			my_sleep(1);
		}
	}
	return (NULL);
}
