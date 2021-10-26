#include "libft.h"

t_intlist	*ft_intlstlast(t_intlist *intlst)
{
	if (!intlst)
		return (NULL);
	while (intlst->next)
		intlst = intlst->next;
	return (intlst);
}
