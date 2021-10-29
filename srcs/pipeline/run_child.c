#include "../../incs/minishell.h"

static bool update_first_pipe(t_pipes *pipes)
{
	if (close(pipes->fd_a[0]) == SYS_ERROR)
	{
		close(pipes->fd_a[1]);
		return (set_sys_error());
	}
	if (dup2(pipes->fd_a[1], STDOUT_FILENO) == SYS_ERROR)
	{
		close(pipes->fd_a[1]);
		return (set_sys_error());
	}
	if (close(pipes->fd_a[1]) == SYS_ERROR)
		return (set_sys_error());
	return (true);
}

static bool	update_mid_pipe(t_pipes *pipes)
{
	if (dup2(pipes->fd_a[0], STDIN_FILENO) == SYS_ERROR)
	{
		close(pipes->fd_a[0]);
		close(pipes->fd_b[0]);
		return (set_sys_error());
	}
	if (close(pipes->fd_a[0]) == SYS_ERROR)
	{
		close(pipes->fd_b[0]);
		return (set_sys_error());
	}
	if (close(pipes->fd_b[0]) == SYS_ERROR)
		return (set_sys_error());
	if (dup2(pipes->fd_b[1], STDOUT_FILENO) == SYS_ERROR)
		return (set_sys_error());
	return (true);
}

static bool	update_last_pipe(t_pipes *pipes)
{
	if (close(pipes->fd_a[1]) == SYS_ERROR && errno != EBADF)
	{
		close(pipes->fd_a[0]);
		return (set_sys_error());
	}
	if (dup2(pipes->fd_a[0], STDIN_FILENO) == SYS_ERROR)
	{	
		close(pipes->fd_a[0]);
		return (set_sys_error());
	}
	if (close(pipes->fd_a[0]) == SYS_ERROR)
		return (set_sys_error());
	return (true);
}

static bool	update_pipes(t_pipes *pipes)
{
	int	rlt;

	rlt = true;
	if (pipes->status == FIRST_PIPE)
		rlt = update_first_pipe(pipes);
	else if (pipes->status == MIDDLE_PIPE)
		rlt = update_mid_pipe(pipes);
	else
		rlt = update_last_pipe(pipes);
	return (rlt);
}

void	run_child(t_node *n, t_pipes *pipes, t_set *set, t_pipe_info *p_info)
{
	extern char	**environ;

	if (!update_pipes(pipes))
		exit(SYS_ERROR);
	if (has_redirection(n))
	{	
		if (!ms_redirection(n, p_info->rdr, set->heredoc_lst))
			exit(REDIRECTION_FAILURE);
	}
	if (p_info->touch)
		exit(EXIT_SUCCESS);
	if (is_buildin(p_info->cmd[0]))
	{
		if (run_builtin_cmd(p_info->cmd, set, false) == FAILURE)
			exit(exec_cmd_error(p_info->cmd[0], NULL, true));
		exit(EXIT_SUCCESS);
	}
	else if (execve(p_info->cmd_path, p_info->cmd, environ) == SYS_ERROR)
		exit(exec_cmd_error(p_info->cmd[0], NULL, false));
}
