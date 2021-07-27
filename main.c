#include "philo.h"

unsigned long get_time(t_data *data)
{
	unsigned long res;
	gettimeofday(&data->t_start, NULL );
//	printf("%ld %d\n", data->t_start.tv_sec, data->t_start.tv_usec);
	res = (data->t_start.tv_sec * 1000 + data->t_start.tv_usec / 1000) -
			data->time_start;
//	printf("start time: %llu\n", data->time_start);
	return (res);
}

void my_usleep(t_data *data)
{
	unsigned long long cur;

	cur = 0;
//	printf("%llu\n", data->time_eat);
//	pthread_mutex_lock(&data->next_p->mut);
//	printf("%llu\n", data->time_eat);
	while (data->time_eat > cur)
	{
		usleep(10);
		gettimeofday(&data->t_start, NULL);
		cur = (data->t_start.tv_sec * 1000 + data->t_start.tv_usec / 1000) -
				data->time_start;
//		printf("cur: %llu\n", cur);
	}
//	printf("cur: %llu\n", cur);
//	pthread_mutex_unlock(&data->next_p->mut);
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
//	printf("start: %llu\n", philo.next_d->time_start);
	my_usleep(philo.next_d);
//	pthread_mutex_lock(&mutex);
//	usleep(1000);
//	printf("current time: %lu\n", get_time(philo.next_d));
	printf("%d\n", philo.name);
	if (philo.name % 2 == 1)// нечетные они всегда первые едят
	{
		pthread_mutex_lock(&philo.left_fork);
		pthread_mutex_lock(&philo.right_fork);
		my_usleep(philo.next_d);
		//get_time(philo.next_d);
		printf("current time: %lu\n philos: %d", get_time(philo.next_d),
		 philo.name);
		pthread_mutex_unlock(&philo.left_fork);
		pthread_mutex_unlock(&philo.right_fork);
		usleep(philo.next_d->time_sleep);
	}

//	while (i < 10)
//	{
//		usleep(1000);
//		printf("%d ", i);
//		i++;
//	}

//	printf("Thread create %d\n", c);
//	c++;
//	pthread_mutex_unlock(&mutex);
	return (NULL);
}

void cycle_create(t_data *data)
{
	pthread_t	*thread;
	t_philo 	*philo;
	int i;

	i = 0;

	philo = (t_philo *)malloc(data->nbr_ph * sizeof(t_philo));
	pthread_mutex_init(&philo->mut, NULL);
	thread = (pthread_t *)malloc(data->nbr_ph * (sizeof(pthread_t)));
	philo->mutex = (pthread_mutex_t *)malloc(data->nbr_ph * sizeof
			(pthread_mutex_t));

	philo->next_d = data;
	data->next_p = philo;
	printf("sa\n");
//	get_time(data);
//	pause();
	while (i < data->nbr_ph)
	{
		pthread_mutex_init(&(philo[i].mutex[i]), NULL);
		philo[i].name = i + 1;
		printf("%d\n", philo[i].name);
		philo[i].right_fork = philo[i].mutex[i];
		printf("oo\n");
		if (i != data->nbr_ph - 1)
		{
			philo[i].left_fork = philo[i].mutex[i + 1];
			printf("a\n");
		}
		else
		{
			philo[i].left_fork = philo[i].mutex[0];
			printf("b\n");
		}
		pthread_create(&thread[i], NULL, &philosoph, (void *)(philo[i]));
		i++;
	}

//	data->next_p->next_d->flag = 1;
	i = 0;
	while (i < data->nbr_ph)
	{
		pthread_join(thread[i++], NULL);
		pthread_mutex_destroy(&data->next_p->mutex[i]);
	}
	free(thread);
	free(data->next_p->mutex);
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
