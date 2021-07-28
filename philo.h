#ifndef PHILO_H
#define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo {
	int 				name;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	int 				dead;
	unsigned long long	last_eat;
	struct s_data		*next_d;
}	t_philo;

typedef struct s_data {
	int					nbr_ph;
	unsigned long long	time_die;
	unsigned long long	time_eat;
	unsigned long long	time_sleep;
	int 				nbr_ph_eat;
	pthread_mutex_t		*mutex;
	struct timeval		t_start;
	unsigned long long	time_start;
	unsigned long long	time_cur;
	int 				flag;
	int 				dead;
	struct s_philo		*next_p;
}	t_data;

int	ft_atoi(char *str);

#endif
