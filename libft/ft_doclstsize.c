#include "libft.h"

int	ft_docstsize(t_doclist *doclst)
{
	int	i;

	i = 0;
	while (doclst)
	{
		doclst = doclst->next;
		i++;
	}
	return (i);
}
