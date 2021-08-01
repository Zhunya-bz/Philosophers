#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Error with arguments\n");
		return (0);
	}
	data.nbr_ph = ft_atoi(argv[1]);
	data.time_die = ft_atoi(argv[2]);
	data.time_eat = ft_atoi(argv[3]);
	data.time_sleep = ft_atoi(argv[4]);
	if (argc == 5)
		data.nbr_eat = -1;
	if (argc == 6)
		data.nbr_eat = ft_atoi(argv[5]);
	create_treads(&data);
	return (0);
}
