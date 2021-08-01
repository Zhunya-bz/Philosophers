#include "philo.h"

void	*philosoph(void *tmp)
{
	t_philo	*philo;

	philo = tmp;
	if (philo->name % 2 == 0)
		my_sleep((int)philo->next_d->time_eat);
	while (philo->next_d->dead != 1 && philo->count_eat != philo
		->next_d->nbr_eat)
	{
		pthread_mutex_lock(philo->left_fork);
		if (print_on_screen(philo, 1))
			return (NULL);
		pthread_mutex_lock(philo->right_fork);
		if (print_on_screen(philo, 2))
			return (NULL);
		phil_eating(philo);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		phil_sleeping(philo);
		phil_thinking(philo);
	}
	return (NULL);
}

static void	create_cycle(t_data *data, t_philo *philo, pthread_t *thread)
{
	int	i;

	i = 0;
	while (i < data->nbr_ph)
	{
		pthread_mutex_init(&data->mutex[i], NULL);
		philo[i].name = i + 1;
		philo[i].right_fork = &data->mutex[i];
		philo[i].last_eat = 0;
		philo[i].count_eat = 0;
		philo[i].next_d = data;
		data->next_p = &philo[i];
		if (i != data->nbr_ph - 1)
			philo[i].left_fork = &data->mutex[i + 1];
		else
			philo[i].left_fork = &data->mutex[0];
		pthread_create(&thread[i], NULL, &philosoph, (void *)(&philo[i]));
		i++;
	}
	usleep(100);
	pthread_create(&thread[i], NULL, &die, (void *)(philo));
}

void	create_treads(t_data *data)
{
	pthread_t	*thread;
	t_philo		*philo;
	int			i;

	i = 0;
	philo = (t_philo *)malloc(data->nbr_ph * sizeof(t_philo));
	thread = (pthread_t *)malloc((data->nbr_ph + 1) * (sizeof(pthread_t)));
	data->mutex = (pthread_mutex_t *)malloc((data->nbr_ph + 1) * sizeof
			(pthread_mutex_t));
	data->dead = 0;
	pthread_mutex_init(&data->mess, NULL);
	data->time_start = get_current_moment();
	if (data->nbr_ph <= 0)
		return ;
	create_cycle(data, philo, thread);
	while (i < data->nbr_ph)
		pthread_mutex_destroy(&data->mutex[i++]);
	pthread_join(thread[i], NULL);
	pthread_mutex_destroy(&data->mess);
	free(thread);
	free(data->mutex);
	free(philo);
}
