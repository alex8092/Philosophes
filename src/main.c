#include <unistd.h>
#include <stdio.h>
#include "ft_philo.h"

pthread_mutex_t	g_mut_status;

int		baguette_in_use(t_philo *phil)
{
	int	ret;

	ret = 0;
	if (!phil->gauche.in_use && phil->last_action != 1 && (phil->phil_gauche->life * 100 / MAX_LIFE >= 80 || phil->life * 100 / MAX_LIFE <= 80))
	{
		phil->bag1 = &phil->gauche;
		phil->gauche.in_use = 1;
		++ret;
	}
	printf("life : %d\n", phil->life * 100 / MAX_LIFE);
	if (!phil->droite->in_use && phil->last_action != 2 && (phil->phil_droite->life * 100 / MAX_LIFE >= 80 || phil->life * 100 / MAX_LIFE <= 80))
	{
		if (phil->bag1)
			phil->bag2 = phil->droite;
		else
			phil->bag1 = phil->droite;
		phil->droite->in_use = 1;
		++ret;
	}
	return (ret);
}

void	release_baguettes(t_philo *phil)
{
	if (phil->bag1)
	{
		phil->bag1->in_use = 0;
		phil->bag1 = NULL;
	}
	if (phil->bag2)
	{
		phil->bag2->in_use = 0;
		phil->bag2 = NULL;
	}
}

void	*thread(void *data)
{
	t_philo	*phil;
	int				n;

	phil = (t_philo *)data;
	while (1)
	{
		//printf("philo %d has %d life\n", phil->n, phil->life);
		if (!phil->eating && !phil->sleeping && !phil->thinking)
		{
			pthread_mutex_lock(&g_mut_status);
			if ((n = baguette_in_use(phil)) > 0)
			{
				if (n == 1)
				{
					printf("phil(%d) thinking\n", phil->n);
					phil->thinking = 1;
					phil->last_action = 1;
				}
				else
				{
					printf("phil(%d) eating\n", phil->n);
					phil->eating = 1;
					phil->last_action = 2;
				}
			}
			else
			{
				printf("phil(%d) sleeping\n", phil->n);
				phil->sleeping = 1;
				phil->last_action = 3;
			}
			pthread_mutex_unlock(&g_mut_status);
		}
	//	usleep(100);
	}
	pthread_exit(NULL);
}

int		loose_life(t_philo **phils, int n)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < 7)
	{
		++phils[i]->actiontime;
		if (!phils[i]->eating)
		{
			phils[i]->life -= n;
			if (phils[i]->life <= 0)
				ret = 1;
			if (phils[i]->thinking && phils[i]->actiontime == THINK_T)
			{
				pthread_mutex_lock(&g_mut_status);
				printf("phil(%d) stop thinking\n", phils[i]->n);
				release_baguettes(phils[i]);
				phils[i]->thinking = 0;
				phils[i]->actiontime = 0;
				pthread_mutex_unlock(&g_mut_status);
			}
			else if (phils[i]->sleeping && phils[i]->actiontime == REST_T)
			{
				pthread_mutex_lock(&g_mut_status);
				printf("phil(%d) stop sleeping\n", phils[i]->n);
				phils[i]->sleeping = 0;
				phils[i]->actiontime = 0;
				pthread_mutex_unlock(&g_mut_status);
			}
		}
		else
		{
			if (phils[i]->actiontime == EAT_T)
			{
				pthread_mutex_lock(&g_mut_status);
				printf("phils(%d) stop eating\n", phils[i]->n);
				release_baguettes(phils[i]);
				phils[i]->eating = 0;
				phils[i]->life = MAX_LIFE;
				phils[i]->actiontime = 0;
				pthread_mutex_unlock(&g_mut_status);
			}
		}
		++i;
	}
	return (ret);
}

int		main(void)
{
	t_philo		*phils[7];
	int			i;
	int			phils_alive;
	pthread_t	threads[7];
	time_t		beg;
	time_t		end;
	t_baguette	*last;
	t_philo		*phil_last;

	last = NULL;
	phil_last = NULL;
	pthread_mutex_init(&g_mut_status, NULL);
	for (i = 0; i < 7; ++i)
	{
		phils[i] = ft_create_philo(last, i);
		phils[i]->phil_droite = phil_last;
		last = &phils[i]->gauche;
		phil_last = phils[i];
		if (i > 0)
			phils[i - 1]->phil_gauche = phils[i];
		if (i == 6)
		{
			phils[0]->droite = last;
			phils[0]->phil_droite = phil_last;
			phils[i]->phil_gauche = phils[0];
		}
	}
	for (i = 0; i < 7; ++i)
		pthread_create(&phils[i]->id, NULL, thread, (void *)phils[i]);
	phils_alive = 1;
	beg = time(NULL);
	while (1)
	{
		if ((end = time(NULL) - beg) > 0)
		{
			beg = time(NULL);
			if (loose_life(phils, end))
				return (0);
		}
		usleep(100);
	}
	pthread_exit(NULL);
	return (0);
}
