#include "libft.h"

t_pidlist	*ft_pidlstlast(t_pidlist *pidlst)
{
	if (!pidlst)
		return (NULL);
	while (pidlst->next)
		pidlst = pidlst->next;
	return (pidlst);
}
