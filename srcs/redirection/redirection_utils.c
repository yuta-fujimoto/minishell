#include "../../incs/minishell.h"

bool	has_redirection(t_node *node)
{
	int	i;

	i = 0;
	while (node->av[i])
	{
		if (is_rdir(node->str_flgs[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	is_rdir(int str_flg)
{
	return (str_flg >= RDIR && str_flg <= LLDIR);
}

bool	is_open_fd(int fd)
{
	struct stat	buf;

	return (fstat(fd, &buf) == 0);
}
