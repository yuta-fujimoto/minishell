#include "libft.h"

t_doclist	*ft_doclstnew(int fds[2], char *delim)
{
	t_doclist	*node;

	node = malloc(sizeof(t_doclist));
	if (!node)
		return (NULL);
	node->fds[0] = fds[0];
	node->fds[1] = fds[1];
	node->delim = delim;
	node->next = NULL;
	return (node);
}
