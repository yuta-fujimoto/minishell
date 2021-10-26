#include "libft.h"

int	ft_intlstsize(t_intlist *intlst)
{
	int	i;

	i = 0;
	while (intlst)
	{
		intlst = intlst->next;
		i++;
	}
	return (i);
}
