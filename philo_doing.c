#include "philo.h"

void	phil_eating(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
	if (print_on_screen(philo, 3))
		return ;
	philo->last_eat = get_time_passed(philo->next_d->time_start);
	my_sleep((int)philo->next_d->time_eat);
	philo->count_eat++;
}

void	phil_sleeping(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
	if (print_on_screen(philo, 4))
		return ;
	my_sleep((int)philo->next_d->time_sleep);
}

void	phil_thinking(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
	if (print_on_screen(philo, 5))
		return ;
}
