#include "ft_philo.h"
#include "ft_common.h"
#include <stdlib.h>

t_philo	*ft_create_philo(t_baguette *droite, int n)
{
	t_philo	*phil;

	phil = (t_philo *)ft_memalloc(sizeof(t_philo));
	if (!phil)
		return (NULL);
	phil->n = n;
	phil->life = MAX_LIFE;
	phil->droite = droite;
	return (phil);
}
