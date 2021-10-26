#include "libft.h"

void	ft_intlstclear(t_intlist **intlst)
{
	t_intlist	*tmp;

	if (!intlst)
		return ;
	while (*intlst)
	{
		tmp = *intlst;
		*intlst = (*intlst)->next;
		free(tmp);
		tmp = NULL;
	}
}
