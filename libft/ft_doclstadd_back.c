#include "libft.h"

bool	ft_doclstadd_back(t_doclist **doclst, t_doclist *new)
{
	if (!new || !doclst)
		return (false);
	if (!*doclst)
		(*doclst) = new;
	else
		ft_doclstlast(*doclst)->next = new;
	return (true);
}
