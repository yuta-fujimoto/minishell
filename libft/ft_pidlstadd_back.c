#include "libft.h"

bool	ft_pidlstadd_back(t_pidlist **pidlst, t_pidlist *new)
{
	if (!new || !pidlst)
		return (false);
	if (!*pidlst)
		(*pidlst) = new;
	else
		ft_pidlstlast(*pidlst)->next = new;
	return (true);
}
