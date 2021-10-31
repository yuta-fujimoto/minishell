#include "libft.h"

void	ft_doclstclear(t_doclist **doclst)
{
	t_doclist	*tmp;

	if (!doclst)
		return ;
	while (*doclst)
	{
		tmp = *doclst;
		*doclst = (*doclst)->next;
		free(tmp);
		tmp = NULL;
	}
}
