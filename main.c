# include "philo.h"

void    create_fork(t_info *info)
{
    int i;
    i = 0;
    while( i < info->params->nb_philo)
    {
        info ->fork[i].fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        if (!info->fork[i].fork)
            exit(1); // need free function
        pthread_mutex_init(info->fork[i].fork, NULL);
        info->fork[i].id = i + 1;
        i++;
    }

}
int elapsed_time(struct timeval *current_time, struct timeval *last_time_eat)
{
    int elapsed_time;
    gettimeofday(current_time, NULL);
    elapsed_time = ((current_time->tv_sec - last_time_eat->tv_sec)*1000 + (current_time->tv_usec -last_time_eat->tv_usec)/1000);
    return (elapsed_time);
}
void *routine_monitor(void *str)
{
    t_info *info = (t_info *)str;
    (void)info;
    // while(1)
    // {
    //     int i = 0;
    //     gettimeofday(info->params->current_time, NULL);

    // }

    return (NULL);
}

void *routine( void *arg)
{   
    t_philo *philo = (t_philo *)arg;
    if (!(philo->id % 2))
    {
        pthread_mutex_lock(philo->left_fork);
        printf("%d philo %d has taken  a fork (left_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        pthread_mutex_lock(philo->right_fork);
        printf("%d philo %d has taken a fork (right fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        gettimeofday(philo->last_time_eat, NULL);
        usleep(philo->params->time_to_eat * 1000);
        pthread_mutex_unlock(philo->left_fork);
        printf("%d philo %d put down a fork (left fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        pthread_mutex_unlock(philo->right_fork);
        printf("%d philo %d put down a fork (right_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        printf("%d philo %d is sleeping\n",elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        usleep(philo->params->time_to_sleep * 1000);
        printf("%d philo %d is thinking\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
    }
    else 
    {
        printf("%d philo %d is sleeping\n", philo->id, elapsed_time(philo->params->current_time, philo->params->start_time));
        usleep(philo->params->time_to_sleep * 1000);
        printf("%d philo %d is thinking\n", philo->id, elapsed_time(philo->params->current_time, philo->params->start_time));
        pthread_mutex_lock(philo->right_fork);
        printf("%d philo %d has taken a fork (right_fork)\n", philo->id, elapsed_time(philo->params->current_time, philo->params->start_time));
        pthread_mutex_lock(philo->left_fork);
        printf("%d philo %d has taken a fork (left_fork)\n", philo->id, elapsed_time(philo->params->current_time, philo->params->start_time));
        gettimeofday(philo->last_time_eat, NULL);
        usleep(philo->params->time_to_eat * 1000);
        pthread_mutex_unlock(philo->right_fork);
        printf("%d philo %d put down a fork (right_fork)\n", philo->id, elapsed_time(philo->params->current_time, philo->params->start_time));
        pthread_mutex_unlock(philo->left_fork);
        printf("%d philo %d put down a fork (left_fork)\n", philo->id, elapsed_time(philo->params->current_time, philo->params->start_time));
    }
    return (NULL);
}
void create_philo(t_info *info)
{
    int i;
    i = 0;
    while(i< info->params->nb_philo)
    {
        info->philo[i].thread = (pthread_t *)malloc(sizeof(pthread_t));
        if (!info->philo[i].thread)
            exit(1); // need free function;
        info->philo[i].params = info->params;
        info->philo[i].id = i+1;
        info->philo[i].right_fork = info->fork[i].fork;
        info->philo[i].left_fork = info->fork[((i +1)%(info->params->nb_philo))].fork;
        if(pthread_create(info->philo[i].thread, NULL, routine, &info->philo[i]))
        {
            printf("Error creating thread\n");
            exit(1); // need free function 
        }
        i++;
    }
    if(pthread_create(info->monitor, NULL, routine_monitor, info))
    {
        printf("Error creating monitor\n");
        exit(1);
    }
    i = 0;
    while(i< info->params->nb_philo)
    {
        if (pthread_join(*(info->philo[i].thread), NULL))
        {
            printf("Error joining thread\n");
            exit (1); // need free function 
        }
        i++;
    }
    if(pthread_join(*(info->monitor), NULL))
    {
        printf("Error joining monitor\n");
        exit(1);
    }
}


void init_info(t_info *info, int argc , char **argv)
{
    info ->params = (t_params *)malloc(sizeof(t_params));
    if (!info->params)
        exit(1); // need free function
    info->params->nb_philo = ft_atoi(argv[1]);
    info->params->time_to_die = ft_atoi(argv[2]);
    info->params->time_to_eat = ft_atoi(argv[3]);
    info->params->time_to_sleep= ft_atoi(argv[4]);
    if (argc == 6)
        info->params->nb_meals = ft_atoi(argv[5]);
    else    
        info->params->nb_meals = -1;
    info ->philo = (t_philo *)malloc(sizeof(t_philo)*info->params->nb_philo);
    if(!info->philo)
        exit(1);  // need free function
    info ->fork = (t_fork *)malloc(sizeof(t_fork)*info->params->nb_philo);
    info->params->start_time = (struct timeval *)malloc(sizeof(struct timeval ));
    info->params->current_time = (struct timeval *)malloc(sizeof(struct timeval ));
    info->monitor = (pthread_t *)malloc(sizeof(pthread_t));
    create_fork(info);
    create_philo(info);
}

void test_content(t_info *info)
{
    printf(" \n test info content\n");
    printf(" nb _philo %d \n",info->params->nb_philo);
    printf(" time_to_die %d \n", info->params->time_to_die);
    printf(" time_to_eat %d \n", info->params->time_to_eat);
    printf(" time to_sleep %d \n", info->params->time_to_sleep);
    printf(" nb_meals %d \n", info->params->nb_meals);
    int i ;
    i = 0;
    while(i <info->params->nb_philo)
    {
        printf("fork id is %d\n", info->fork[i].id);
        i++;
    }

}
int main(int argc, char **argv)
{
    t_info *info;

    info = NULL;
    if (argc != 5 && argc != 6)
        ft_error(0);
    info = (t_info *)malloc(sizeof(t_info));
    if(!info)
        exit(1);  // need free function
    init_info(info, argc, argv);
    // test_content(info);
}