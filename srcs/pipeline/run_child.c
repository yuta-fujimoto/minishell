#include "../../incs/minishell.h"

static void	update_pipes(t_pipes *pipes)
{
	if (pipes->status == FIRST_PIPE)
	{
		close(pipes->fd_a[0]);
		dup2(pipes->fd_a[1], STDOUT_FILENO);
		close(pipes->fd_a[1]);
	}
	else if (pipes->status == MIDDLE_PIPE)
	{
		dup2(pipes->fd_a[0], STDIN_FILENO);
		close(pipes->fd_a[0]);
		close(pipes->fd_b[0]);
		dup2(pipes->fd_b[1], STDOUT_FILENO);
	}
	else
	{
		close(pipes->fd_a[1]);
		dup2(pipes->fd_a[0], STDIN_FILENO);
		close(pipes->fd_a[0]);
	}
}

void	run_child(t_node *n, t_pipes *pipes, t_set *set, t_pipe_info *p_info)
{
	extern char	**environ;

	update_pipes(pipes);
	if (has_redirection(n))
	{
		if (!ms_redirection(n, p_info->rdr))
			exit(REDIRECTION_FAILURE);
	}	
	if (p_info->touch)
		exit(EXIT_SUCCESS);
	if (is_buildin(p_info->cmd[0]))
	{
		if (run_builtin_cmd(p_info->cmd, set) == FAILURE)
			exit(exec_cmd_error(p_info->cmd[0], NULL, true));
		exit(EXIT_SUCCESS);
	}
	else if (execve(p_info->cmd_path, p_info->cmd, environ) == SYS_ERROR)
		exit(exec_cmd_error(p_info->cmd[0], NULL, false));
}
