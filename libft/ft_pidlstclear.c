#include "libft.h"

void	ft_pidlstclear(t_pidlist **pidlst)
{
	t_pidlist	*tmp;

	if (!pidlst)
		return ;
	while (*pidlst)
	{
		tmp = *pidlst;
		*pidlst = (*pidlst)->next;
		free(tmp);
		tmp = NULL;
	}
}
