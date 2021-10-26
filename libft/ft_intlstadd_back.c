#include "libft.h"

bool	ft_intlstadd_back(t_intlist **intlst, t_intlist *new)
{
	if (!new || !intlst)
		return (false);
	if (!*intlst)
		(*intlst) = new;
	else
		ft_intlstlast(*intlst)->next = new;
	return (true);
}
