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

unsigned long get_time_0(t_data *data)
{
	unsigned long res;
	gettimeofday(&data->t_start, NULL );
//	printf("%ld %d\n", data->t_start.tv_sec, data->t_start.tv_usec);
	res = (data->t_start.tv_sec * 1000 + data->t_start.tv_usec / 1000) - data->time_start;
//	printf("time: %llu\n", res+data->time_start);
//	printf("start time: %llu\n", data->time_start);
	return (res);
}

void my_usleep(t_data *data, unsigned long long var, unsigned long long start)
{
	unsigned long long cur;

	cur = 0;
//	printf("%llu\n", data->time_eat);
//	pthread_mutex_lock(&data->next_p->mut);
//	printf("get: %llu\n", var);
	while (var > cur)
	{
		usleep(50);
		gettimeofday(&data->t_start, NULL);
		cur = (data->t_start.tv_sec * 1000 + data->t_start.tv_usec / 1000) -
				start - data->time_start;
	}
//	printf("cur: %llu\n", cur + data->time_start);
//	pthread_mutex_unlock(&data->next_p->mut);
}

void phil_eating(t_philo	philo)
{
	if (philo.next_d->dead == 1)
		return ;
	printf("%lu: %d is eating\n", get_time_0(philo.next_d),
		   philo.name);
	philo.last_eat = get_time_0(philo.next_d);
	my_usleep(philo.next_d, philo.next_d->time_eat, get_time_0(philo.next_d));
}

void phil_sleeping(t_philo	philo)
{
	if (philo.next_d->dead == 1)
		return ;
	printf("%lu: %d is sleeping\n",
		   get_time_0(philo.next_d), philo.name);
	my_usleep(philo.next_d, philo.next_d->time_sleep, get_time_0(philo.next_d));
}

void phil_thinking(t_philo	philo)
{
	if (philo.next_d->dead == 1)
		return ;
	printf("%lu: %d is thinking\n",
		   get_time_0(philo.next_d), philo.name);
}

void *philosoph(void *tmp)
{
	t_philo	philo;

	philo = *(t_philo *)tmp;
//	while (1)
//	{
//		if (philo.next_d->flag == 1)
//			break;
//	}
	gettimeofday(&philo.next_d->t_start, NULL);
	philo.next_d->time_start = (philo.next_d->t_start.tv_sec * 1000 +
			philo.next_d->t_start.tv_usec / 1000);
	while (philo.next_d->dead != 1)
	{
		if (philo.name % 2 == 0)// нечетные они всегда первые едят
			my_usleep(philo.next_d, 10, 0);
		pthread_mutex_lock(philo.left_fork);
		printf("%lu: %d has taken a left fork\n", get_time_0(philo.next_d),
			   philo.name);
		pthread_mutex_lock(philo.right_fork);
		printf("%lu: %d has taken a right fork\n", get_time_0(philo.next_d),
			   philo.name);
		phil_eating(philo);
		pthread_mutex_unlock(philo.left_fork);
		pthread_mutex_unlock(philo.right_fork);
		phil_sleeping(philo);
		phil_thinking(philo);
	}

//	pthread_mutex_unlock(&mutex);
	return (NULL);
}

void *die(void *tmp)
{
	t_philo	*philo;
	t_data	*data;
	int 	i;
	int 	j;

	i = 0;
	j = 0;
	philo = tmp;
	data = philo[0].next_d;
	while (philo[i].dead != 1)
	{
		j = 0;
		while (j < philo[i].next_d->nbr_ph)
		{
//			printf("%d\n", philo[i].name);
			if (philo[i].next_d->time_die < get_time_0(philo[i].next_d) -
			philo[i].last_eat)
			{
				printf("last eat: %llu p: %d\n", philo[i].last_eat, philo[i]
				.name);
				philo[i].next_d->dead = 1;
				printf("%lu: %d died\n", get_time_0(philo[i].next_d), philo[i]
				.name);
				return (NULL);
			}
			j++;
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
	data->mutex = (pthread_mutex_t *)malloc(data->nbr_ph * sizeof
			(pthread_mutex_t));
	while (i < data->nbr_ph)
	{
		pthread_mutex_init(&data->mutex[i], NULL);
//		pthread_mutex_init(&philo[i].left_fork, NULL);
		philo[i].name = i + 1;
		philo[i].right_fork = &data->mutex[i];
		philo[i].dead = 0;
		philo[i].last_eat = 0;
		philo[i].next_d = data;
		data->next_p = &philo[i];
		if (i != data->nbr_ph - 1)
			philo[i].left_fork = &data->mutex[i + 1];
		else
			philo[i].left_fork = &data->mutex[0];
		pthread_create(&thread[i], NULL, &philosoph, (void *)(&philo[i]));
		i++;
	}
//	sleep(1);
	pthread_create(&thread[i], NULL, &die, (void *)(philo));

//	data->next_p->next_d->flag = 1;
	i = 0;
	while (i < data->nbr_ph)
	{
		pthread_join(thread[i++], NULL);
		pthread_mutex_destroy(&data->mutex[i]);
	}
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
