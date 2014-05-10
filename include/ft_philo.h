#ifndef FT_PHILO_H
# define FT_PHILO_H

# include <pthread.h>

# define MAX_LIFE	10
# define EAT_T		3
# define REST_T		1
# define THINK_T	2
# define TIMEOUT	60

typedef struct s_philo		t_philo;
typedef struct s_baguette	t_baguette;

struct			s_baguette
{
	int			in_use;
};

struct			s_philo
{
	t_baguette	gauche;
	t_philo		*phil_gauche;
	int			life;
	int			n;
	int			thinking;
	int			sleeping;
	int			eating;
	int			last_action;
	int			actiontime;
	t_baguette	*bag1;
	t_baguette	*bag2;
	pthread_t	id;
	t_baguette	*droite;
	t_philo		*phil_droite;
};

t_philo			*ft_create_philo(t_baguette *droite, int n);

#endif
