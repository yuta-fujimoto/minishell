#include "../../incs/minishell.h"

bool	close_fd(int fd, int rlt)
{
	int	ret;

	ret = 0;
	if (fd != -1)
		ret = close(fd);
	if (ret == SYS_ERROR || rlt == FAILURE)
		return (false);
	return (true);
}

bool	end_stdio_fd(t_redir *redir, int rlt)
{
	int	ret;

	ret = 0;
	if (is_open_fd(redir->safe_out))
		ret = dup2(redir->safe_out, STDOUT_FILENO);
	if (is_open_fd(redir->safe_in))
		ret = dup2(redir->safe_in, STDIN_FILENO);
	if (ret == SYS_ERROR || rlt == FAILURE)
		return (false);
	return (true);
}

bool	end_redirection(char **cmd, t_redir *redir, int rlt)
{
	if (cmd)
	{
		free(cmd);
		cmd = NULL;
	}
	if (!end_stdio_fd(redir, rlt))
		rlt = FAILURE;
	if (is_open_fd(redir->new_in))
	{
		if (!close_fd(redir->new_in, rlt))
			rlt = FAILURE;
	}
	if (!close_fd(redir->new_out, rlt))
		rlt = FAILURE;
	if (!close_fd(redir->safe_in, rlt))
		rlt = FAILURE;
	if (!close_fd(redir->safe_out, rlt))
		rlt = FAILURE;
	return (rlt);
}
