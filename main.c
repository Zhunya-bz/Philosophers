#include "philo.h"

//unsigned long get_time(t_data *data)
//{
//	unsigned long res;
//	gettimeofday(&data->t_start, NULL );
////	printf("%ld %d\n", data->t_start.tv_sec, data->t_start.tv_usec);
//	res = (data->t_start.tv_sec * 1000 + data->t_start.tv_usec / 1000);
////	printf("time: %llu\n", res+data->time_start);
////	printf("start time: %llu\n", data->time_start);
//	return (res);
//}

//unsigned long long get_time_0(t_data *data)
//{
//	unsigned long long res;
//
//	gettimeofday(&data->t_start, NULL );
//	res = (data->t_start.tv_sec * 1000 + data->t_start.tv_usec / 1000) - data->time_start;
//	return (res);
//}

//void my_usleep(t_data *data, unsigned long long var, unsigned long long start)
//{
//	unsigned long long cur;
//
//	cur = 0;
//	while (var > cur)
//	{
//		usleep(50);
//		gettimeofday(&data->t_start, NULL);
//		cur = (data->t_start.tv_sec * 1000 + data->t_start.tv_usec / 1000) -
//				start - data->time_start;
//	}
//}


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
//	pthread_mutex_lock(&philo->next_d->mess);
//	pthread_mutex_lock(&philo->next_d->mess);
	if (c == 0)
	{
		printf(RED"%d: %d died\n", get_time_passed(philo->next_d->time_start),
			   philo->name);
		return (1);
	}
	if (philo->next_d->dead == 1)
		return (1);
	pthread_mutex_lock(&philo->next_d->mess);
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
//	printf(GREEN "%d: %d is eating\n", get_time_passed
//	(philo->next_d->time_start),philo->name);
//	philo->last_eat = get_time_passed(philo->next_d->time_start);
	if (print_on_screen(philo, 3))
		return ;
	philo->last_eat = get_time_passed(philo->next_d->time_start);
//	printf("LAST: %llu %d\n", philo->last_eat,  philo->name);
//	my_usleep(philo->next_d, philo->next_d->time_eat, get_time_0
//	(philo->next_d));
	my_sleep((int)philo->next_d->time_eat);
}

void phil_sleeping(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
//	printf(BLUE "%d: %d is sleeping\n",
//		   get_time_passed(philo->next_d->time_start), philo->name);
	if (print_on_screen(philo, 4))
		return ;
//	my_usleep(philo->next_d, philo->next_d->time_sleep, get_time_0
//	(philo->next_d));
	my_sleep((int)philo->next_d->time_sleep);
}

void phil_thinking(t_philo	*philo)
{
	if (philo->next_d->dead == 1)
		return ;
//	printf(RESET "%d: %d is thinking\n",
//		   get_time_passed(philo->next_d->time_start), philo->name);
	if (print_on_screen(philo, 5))
		return ;
}

void *philosoph(void *tmp)
{
	t_philo	*philo;

	philo = tmp;
//	while (1)
//	{
//		if (philo.next_d->flag == 1)
//			break;
//	}
	while (philo->next_d->dead != 1)
	{
		if (philo->name % 2 == 0)// нечетные они всегда первые едят
			usleep(500);
		pthread_mutex_lock(philo->left_fork);
//		printf(MAGENTA "%d: %d has taken a left fork\n", get_time_passed
//		(philo->next_d->time_start),philo->name);
		if (print_on_screen(philo, 1))
			return (NULL);
		if (philo->next_d->nbr_ph == 1)
		{
			while (1)
				if (philo->next_d->dead == 1)
					return (NULL);
		}
		pthread_mutex_lock(philo->right_fork);
//		printf(MAGENTA "%d: %d has taken a right fork\n", get_time_passed
//		(philo->next_d->time_start),philo->name);
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

void *die(void *tmp)
{
	t_philo	*philo;
	t_data	*data;
	int 	i;

	philo = tmp;
	data = philo[0].next_d;
	while (1)
	{
		i = 0;
		while (i < data->nbr_ph)
		{
			if (data->time_die + 1 <= get_time_passed(data->time_start) -
			philo[i].last_eat)
			{
				data->dead = 1;
//				printf(RED"%d: %d died\n", get_time_passed(data->time_start),
//					   philo[i].name);
				print_on_screen(&philo[i], 0);
				return (NULL);
			}
			i++;
			//TODO: Если детектирвоание смерти будет не точным изменить
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
//		pthread_mutex_init(&philo[i].left_fork, NULL);
		philo[i].name = i + 1;
		philo[i].right_fork = &data->mutex[i];
		philo[i].last_eat = 0;
		philo[i].next_d = data;
//		philo[i].next_d = data;
		data->next_p = &philo[i];
		if (i != data->nbr_ph - 1)
			philo[i].left_fork = &data->mutex[i + 1];
		else
			philo[i].left_fork = &data->mutex[0];

		i++;
	}
	i = 0;
	while (i < data->nbr_ph)
	{
		pthread_create(&thread[i], NULL, &philosoph, (void *) (&philo[i]));
		i++;
	}
//	usleep(100);
	pthread_create(&thread[i], NULL, &die, (void *)(philo));
//	data->next_p->next_d->flag = 1;
//	sleep(20);
	i = 0;
	while (i < data->nbr_ph)
	{
		pthread_join(thread[i], NULL);
		pthread_mutex_destroy(&data->mutex[i]);
		i++;
	}
	pthread_join(thread[i], NULL);
	// Ending

	free(thread);
	free(data->mutex);
}


int main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5)
	{
		data.nbr_ph = ft_atoi(argv[1]);
		data.time_die = ft_atoi(argv[2]);
		data.time_eat = ft_atoi(argv[3]);
		data.time_sleep = ft_atoi(argv[4]);
		cycle_create(&data);
	}
	else if (argc == 6)
	{
		data.nbr_ph = ft_atoi(argv[1]);
		data.time_die = ft_atoi(argv[2]);
		data.time_eat = ft_atoi(argv[3]);
		data.time_sleep = ft_atoi(argv[4]);
		data.nbr_ph_eat = ft_atoi(argv[5]);
	}
	else
		printf("Error with arguments\n");

	return (0);
}
