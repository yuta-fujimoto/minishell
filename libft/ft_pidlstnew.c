#include "libft.h"

t_pidlist	*ft_pidlstnew(pid_t pid)
{
	t_pidlist	*pidnode;

	pidnode = malloc(sizeof(t_pidlist));
	if (!pidnode)
		return (NULL);
	pidnode->pid = pid;
	pidnode->next = NULL;
	return (pidnode);
}
