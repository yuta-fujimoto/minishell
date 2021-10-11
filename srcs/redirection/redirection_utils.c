#include "../../incs/minishell.h"

bool	has_redirection(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (is_rdir(av[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	is_rdir(char *cmd_i)
{
	return (cmd_i[0] == '>' || cmd_i[0] == '<');
}

bool	is_open_fd(int fd)
{
	struct stat	buf;

	return (fstat(fd, &buf) == 0);
}
