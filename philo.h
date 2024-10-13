#ifndef PHILO_H
#define philo_h

# include <stdio.h>
# include <sys/time.h>
# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_params
{
    int nb_philo;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int nb_meals;
    struct timeval *start_time;
    struct timeval *current_time;
}t_params;

typedef struct s_philo
{
    int id;
    int nb_times_eat;
    struct timeval *last_time_eat;
    pthread_t *thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct timeval  *current_time;
    t_params *params;
}   t_philo;


typedef struct s_fork
{
    int id;
    pthread_mutex_t *fork;
}   t_fork;

typedef struct s_info
{
    t_params    *params;
    t_philo     *philo;
    t_fork      *fork;
    pthread_t   *monitor;
}   t_info;



void ft_error(int n);
long ft_atoi(char *str);

#endif