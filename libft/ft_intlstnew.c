#include "libft.h"

t_intlist	*ft_intlstnew(int num)
{
	t_intlist	*numnode;

	numnode = malloc(sizeof(t_intlist));
	if (!numnode)
		return (NULL);
	numnode->num = num;
	numnode->next = NULL;
	return (numnode);
}
