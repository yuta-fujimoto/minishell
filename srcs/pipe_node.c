#include "../incs/minishell.h"

bool		minishell_error(void)
{
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

static bool	pipe_exit_failure(int fd1, int fd2)
{
	if (fd1)
		close(fd1);
	if (fd2)
		close(fd2);
	return (FAILURE);
}

void	update_pipes(t_pipes *pipes)
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

bool	wait_options(pid_t pid)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	if (!WIFEXITED(wstatus))
		return (minishell_error());
	return (SUCCESS);
}

static void	update_status(t_node node, t_pipes *pipes)
{
	if (node.flgs == PIPE)
		pipes->status = MIDDLE_PIPE;
	else
		pipes->status = END_PIPE;
}

static void	swap_fds(t_pipes *pipes)
{
	int tmp[2];

	tmp[0] = pipes->fd_a[0];
	tmp[1] = pipes->fd_a[1];
	pipes->fd_a[0] = pipes->fd_b[0];
	pipes->fd_a[1] = pipes->fd_b[1];
	pipes->fd_b[0] = tmp[0];
	pipes->fd_b[1] = tmp[1];
}

static bool pipe_nodes(t_tree *parent, t_pipes *pipes, t_set *set)
{
    if (pipes->status == FIRST_PIPE)
    {
		if (run_shell_cmd(parent->left->node, pipes, set) == FAILURE)
            return (pipe_exit_failure(pipes->fd_a[0], pipes->fd_a[1]));
        close(pipes->fd_a[1]);
        update_status(parent->right->node, pipes);
        if (pipes->status == MIDDLE_PIPE)
            pipe_nodes(parent->right, pipes, set);
        else
            pipe_nodes(parent, pipes, set);
    }
    else if (pipes->status == MIDDLE_PIPE)
    {
        if (pipe(pipes->fd_b) == -1)
            return (pipe_exit_failure(pipes->fd_a[0], pipes->fd_a[1]));
		if (run_shell_cmd(parent->left->node, pipes, set) == FAILURE)
            return (pipe_exit_failure(pipes->fd_a[0], pipes->fd_b[1]));
        close(pipes->fd_a[0]);
        close(pipes->fd_b[1]);
        swap_fds(pipes);
        update_status(parent->right->node, pipes);
        if (pipes->status == MIDDLE_PIPE)
            pipe_nodes(parent->right, pipes, set);
        else
            pipe_nodes(parent, pipes, set);
    }else
    {
		if (run_shell_cmd(parent->right->node, pipes, set) == SUCCESS)
            return (pipe_exit_failure(pipes->fd_a[0], 0));
        close(pipes->fd_a[0]);
    }
    return (SUCCESS);
}

bool    ms_pipe(t_tree *parent, t_pipes *pipes, t_set *set)
{
    if (pipe(pipes->fd_a) == -1)
        return (FAILURE);
    pipes->status = FIRST_PIPE;
    if (pipe_nodes(parent, pipes, set) == FAILURE)
        return (FAILURE);
    return (SUCCESS);
}
