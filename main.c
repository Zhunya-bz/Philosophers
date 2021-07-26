#include "philo.h"

void *func()
{
	printf("Thread create\n");
}

void cycle_create(t_data *data)
{
	pthread_t *thread;
	int i;

	i = 0;
	thread = (pthread_t *)malloc(data->nbr_ph * (sizeof(pthread_t)));
	while (i < data->nbr_ph)
	{
		pthread_create(&thread[i], NULL, &func, NULL);
		i++;
	}
	i = 0;
	while (i < data->nbr_ph)
	{
		pthread_join(&thread[i], NULL);
		i++;
	}
}


int main(int argc, char **argv)
{
	int	i;
	t_data	data;

	if (argc == 5)
	{
		data.nbr_ph = ft_atoi(argv[1]);
		data.time_die = ft_atoi(argv[2]);
		data.time_eat = ft_atoi(argv[3]);
		data.time_sleep = ft_atoi(argv[4]);
		cycle_create();
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
