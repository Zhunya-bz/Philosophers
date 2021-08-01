#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define RED     "\x1b[31m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define BLUE    "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN    "\x1b[36m"
# define RESET   "\x1b[0m"

typedef struct s_philo {
	int					name;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	long				last_eat;
	int					count_eat;
	struct s_data		*next_d;
}	t_philo;

typedef struct s_data {
	int					nbr_ph;
	long				time_die;
	long				time_eat;
	long				time_sleep;
	int					nbr_eat;
	pthread_mutex_t		*mutex;
	pthread_mutex_t		mess;
	long				time_start;
	int					eat_all;
	int					dead;
	struct s_philo		*next_p;
}	t_data;

int		ft_atoi(char *str);
long	get_current_moment(void);
int		get_time_passed(long starting_point);
void	my_sleep(int ms);
void	phil_eating(t_philo	*philo);
void	phil_sleeping(t_philo	*philo);
void	phil_thinking(t_philo	*philo);
int		print_on_screen(t_philo	*philo, int c);
void	*die(void *tmp);
void	create_treads(t_data *data);
void	*philosoph(void *tmp);

#endif
