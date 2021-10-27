#include "libft.h"

t_doclist	*ft_doclstlast(t_doclist *doclst)
{
	if (!doclst)
		return (NULL);
	while (doclst->next)
		doclst = doclst->next;
	return (doclst);
}
