#include "philo.h"

static void	print_2(t_philo	*philo, int c)
{
	if (c == 1)
		printf(MAGENTA "%d: %d has taken a left fork\n", get_time_passed
			(philo->next_d->time_start), philo->name);
	else if (c == 2)
		printf(MAGENTA "%d: %d has taken a right fork\n", get_time_passed
			(philo->next_d->time_start), philo->name);
	else if (c == 3)
		printf(GREEN "%d: %d is eating\n", get_time_passed
			(philo->next_d->time_start), philo->name);
	else if (c == 4)
		printf(BLUE "%d: %d is sleeping\n",
			get_time_passed(philo->next_d->time_start), philo->name);
	else if (c == 5)
		printf(RESET "%d: %d is thinking\n",
			get_time_passed(philo->next_d->time_start), philo->name);
}

int	print_on_screen(t_philo	*philo, int c)
{
	pthread_mutex_lock(&philo->next_d->mess);
	if (c == 0)
	{
		printf(RED"%d: %d died\n", get_time_passed(philo->next_d->time_start),
			   philo->name);
		pthread_mutex_unlock(&philo->next_d->mess);
		return (1);
	}
	if (philo->next_d->dead == 1 || philo->next_d->eat_all == philo
		->next_d->nbr_eat)
	{
		pthread_mutex_unlock(&philo->next_d->mess);
		return (1);
	}
	print_2(philo, c);
	pthread_mutex_unlock(&philo->next_d->mess);
	return (0);
}
