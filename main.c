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
    // printf(" current time is : in sec %ld and in usec %ld \n", current_time->tv_sec, current_time->tv_usec);
    // printf(" last time eat is : in sec %ld and in usec %ld \n", last_time_eat->tv_sec, last_time_eat->tv_usec);
    elapsed_time = ((current_time->tv_sec - last_time_eat->tv_sec)*1000 + (current_time->tv_usec -last_time_eat->tv_usec)/1000);
    return (elapsed_time);
}

int check_times(t_info *info)
{
    int i = 0;
    while(i < info->params->nb_philo)
    {
        if (info->params->nb_meals != -1 && info->params->nb_meals == info->philo->nb_times_eat)
            i++;
        else
            return (0);
    }
    return (1);
}
void *routine_monitor(void *str)
{
    t_info *info = (t_info *)str;
    while(1)
    {
        int i = 0;
        if (check_times(info))
        {
            printf(" simulation stops\n");
            exit(1);
        }
        while(i < info->params->nb_philo)
        {
            int t = elapsed_time(info->philo[i].current_time, info->philo[i].last_time_eat);
            if (t > info->params->time_to_die)
            {
                printf("\n %d \n", t);
                printf(" philo %d is dead\n", info->philo[i].id);
                exit(1);
            }
            i++;
        }
    }
    return (NULL);
}

void *routine( void *arg)
{   
    t_philo *philo = (t_philo *)arg;
        if (!(philo->id % 2))
        usleep(10000);
    while(1)
    {
        // {
            pthread_mutex_lock(philo->left_fork);
            printf("%d philo %d has taken  a fork (left_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
            pthread_mutex_lock(philo->right_fork);
            printf("%d philo %d has taken a fork (right fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
            gettimeofday(philo->last_time_eat, NULL);
            printf("%d philo %d is eating\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
            usleep(philo->params->time_to_eat * 1000);
            if (philo->params->nb_meals != -1)
                philo->nb_times_eat ++;
            pthread_mutex_unlock(philo->left_fork);
            printf("%d philo %d put down a fork (left fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
            pthread_mutex_unlock(philo->right_fork);
            printf("%d philo %d put down a fork (right_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
            printf("%d philo %d is sleeping\n",elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
            usleep(philo->params->time_to_sleep * 1000);
            printf("%d philo %d is thinking\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        // }
        // else 
        // {
        //     printf("%d philo %d is sleeping\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        //     usleep(philo->params->time_to_sleep * 1000);
        //     printf("%d philo %d is thinking\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        //     pthread_mutex_lock(philo->right_fork);
        //     printf("%d philo %d has taken a fork (right_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        //     pthread_mutex_lock(philo->left_fork);
        //     printf("%d philo %d has taken a fork (left_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        //     gettimeofday(philo->last_time_eat, NULL);
        //     printf("%d philo %d is eating\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        //     usleep(philo->params->time_to_eat * 1000);
        //     if (philo->params->nb_meals != -1)
        //         philo->nb_times_eat ++;
        //     pthread_mutex_unlock(philo->right_fork);
        //     printf("%d philo %d put down a fork (right_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        //     pthread_mutex_unlock(philo->left_fork);
        //     printf("%d philo %d put down a fork (left_fork)\n", elapsed_time(philo->params->current_time, philo->params->start_time), philo->id);
        // }
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
        info->philo[i].last_time_eat = (struct timeval *)malloc(sizeof(struct timeval));
        gettimeofday(info->philo[i].last_time_eat, NULL);
        info->philo[i].current_time = (struct timeval *)malloc(sizeof(struct timeval));
        if (!info->philo[i].thread)
            exit(1); // need free function;
        info->philo[i].params = info->params;
        info->philo[i].id = i+1;
        info->philo[i].right_fork = info->fork[i].fork;
        info->philo[i].left_fork = info->fork[((i +1)%(info->params->nb_philo))].fork;
        info->philo[i].nb_times_eat = 0;
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
    gettimeofday(info->params->start_time, NULL);
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