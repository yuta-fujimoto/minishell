#include "libft.h"

int	ft_pidlstsize(t_pidlist *pidlst)
{
	int	i;

	i = 0;
	while (pidlst)
	{
		pidlst = pidlst->next;
		i++;
	}
	return (i);
}
