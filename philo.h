#ifndef PHILO_H
#define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct s_philo
{

}	t_philo;

typedef struct s_data {
	int					nbr_ph;
	unsigned long long	time_die;
	unsigned long long	time_eat;
	unsigned long long	time_sleep;
	int 				nbr_ph_eat;
}	t_data;

#endif
