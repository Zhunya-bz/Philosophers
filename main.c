#include "philo.h"

long get_current_moment(void)
{
	struct timeval t_start;

	gettimeofday(&t_start, NULL);
	return (t_start.tv_sec * 1000 + t_start.tv_usec / 1000);
}

int get_time_passed(long starting_point) {
	return (int)(get_current_moment() - starting_point);
}


int print_on_screen(t_philo	*philo, int c)
{
	pthread_mutex_lock(&philo->next_d->mess);
	if (c == 0)
	{
		printf(RED"%d: %d died\n", get_time_passed(philo->next_d->time_start),
			   philo->name);
		pthread_mutex_unlock(&philo->next_d->mess);
		return (1);
	}
	if (philo->next_d->dead == 1 || philo->next_d->eat_all == philo->next_d->nbr_eat)
	{
		pthread_mutex_unlock(&philo->next_d->mess);
		return (1);
	}
	if (c == 1)
		printf(MAGENTA "%d: %d has taken a left fork\n", get_time_passed
				(philo->next_d->time_start), philo->name);
	else if (c == 2)
		printf(MAGENTA "%d: %d has taken a right fork\n", get_time_passed
				(philo->next_d->time_start), philo->name);
	else if (c == 3)
		printf(GREEN "%d: %d is eating\n", get_time_passed
				(philo->next_d->time_start),philo->name);
	else if (c == 4)
		printf(BLUE "%d: %d is sleeping\n",
			   get_time_passed(philo->next_d->time_start), philo->name);
	else if (c == 5)
		printf(RESET "%d: %d is thinking\n",
			   get_time_passed(philo->next_d->time_start), philo->name);
	pthread_mutex_unlock(&philo->next_d->mess);
	return (0);
}

void my_sleep(int ms)
{
	long start;

	start = get_current_moment();
	while (get_current_moment() - start < ms)
		usleep(50);
}

void phil_eating(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
	if (print_on_screen(philo, 3))
		return ;
	philo->last_eat = get_time_passed(philo->next_d->time_start);
	my_sleep((int)philo->next_d->time_eat);
	philo->count_eat++;
}

void phil_sleeping(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
	if (print_on_screen(philo, 4))
		return ;
	my_sleep((int)philo->next_d->time_sleep);
}

void phil_thinking(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
	if (print_on_screen(philo, 5))
		return ;
}

void *philosoph(void *tmp)
{
	t_philo	*philo;

	philo = tmp;
	if (philo->name % 2 == 0)// нечетные они всегда первые едят
		my_sleep((int)philo->next_d->time_eat);
	while (philo->next_d->dead != 1 && philo->count_eat != philo->next_d->nbr_eat)
	{
		pthread_mutex_lock(philo->left_fork);
		if (print_on_screen(philo, 1))
			return (NULL);
		pthread_mutex_lock(philo->right_fork);
		if (print_on_screen(philo, 2))
			return (NULL);
		phil_eating(philo);
//		philo->count_eat++;
//		if (philo->count_eat == philo->next_d->nbr_eat)
//		{
//			printf("All eat\n");
//			return (NULL);
//		}
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		phil_sleeping(philo);
		phil_thinking(philo);
	}
	return (NULL);
}

void *die(void *tmp)
{
	t_philo	*philo;
	t_data	*data;
	int 	i;

	philo = tmp;
	data = philo[0].next_d;
	while (1)
	{
		data->eat_all = 0;
		i = 0;
		while (i < data->nbr_ph)
		{
			if (data->time_die + 1 <= get_time_passed(data->time_start) -
			philo[i].last_eat)
			{
				data->dead = 1;
				print_on_screen(&philo[i], 0);
				return (NULL);
			}
			if (philo[i].count_eat == data->nbr_eat)
			{
				data->eat_all++;
				if (data->eat_all == data->nbr_ph)
				{
				printf(YELLOW"%d: all the philosophers ate\n",
					   get_time_passed
							   (data->time_start));
					return (NULL);
				}
			}
			i++;
			my_sleep(1);
		}
	}
	return (NULL);
}

void cycle_create(t_data *data)
{
	pthread_t	*thread;
	t_philo 	*philo;
	int i;

	i = 0;
	philo = (t_philo *)malloc(data->nbr_ph * sizeof(t_philo));
	thread = (pthread_t *)malloc((data->nbr_ph + 1) * (sizeof(pthread_t)));
	data->mutex = (pthread_mutex_t *)malloc((data->nbr_ph + 1) * sizeof
			(pthread_mutex_t));
	data->dead = 0;
	pthread_mutex_init(&data->mess, NULL);
	data->time_start = get_current_moment();
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
		pthread_create(&thread[i], NULL, &philosoph, (void *) (&philo[i]));
		i++;
	}
	usleep(100);
	pthread_create(&thread[i], NULL, &die, (void *)(philo));
	i = 0;
	while (i < data->nbr_ph)
	{
		pthread_mutex_destroy(&data->mutex[i]);
		i++;
	}
	pthread_join(thread[i], NULL);
	pthread_mutex_destroy(&data->mess);
	// Ending
	free(thread);
	free(data->mutex);
	free(philo);
}


int main(int argc, char **argv)
{
	t_data	data;

	data.nbr_ph = ft_atoi(argv[1]);
	data.time_die = ft_atoi(argv[2]);
	data.time_eat = ft_atoi(argv[3]);
	data.time_sleep = ft_atoi(argv[4]);
	if (argc == 5)
	{
		data.nbr_eat = -1;
		cycle_create(&data);
	}
	else if (argc == 6)
	{
		data.nbr_eat = ft_atoi(argv[5]);
		cycle_create(&data);
	}
	else
		printf("Error with arguments\n");
	return (0);
}
