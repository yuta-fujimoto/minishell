#include "../../incs/minishell.h"

bool	pipe_exit_failure(t_pipes *pipes)
{
	if (pipes->status == FIRST_PIPE)
	{
		close (pipes->fd_a[0]);
		close (pipes->fd_a[1]);
	}
	else if (pipes->status == MIDDLE_PIPE)
	{	
		close (pipes->fd_a[0]);
		close (pipes->fd_b[0]);
		close (pipes->fd_b[1]);
	}
	else
		close (pipes->fd_a[0]);
	return (FAILURE);
}

bool	close_pipes(t_pipes *pipes)
{
	bool	rlt;

	rlt = true;
	if (pipes->status == FIRST_PIPE)
	{
		if (!close_fd(pipes->fd_a[1], SUCCESS))
			rlt = false;
	}
	else if (pipes->status == MIDDLE_PIPE)
	{
		if (!close_fd(pipes->fd_a[0], SUCCESS))
			rlt = false;
		if (!close_fd(pipes->fd_b[1], SUCCESS))
			rlt = false;
	}
	else
	{
		if (!close_fd(pipes->fd_a[0], SUCCESS))
			rlt = false;
	}
	return (rlt);
}

void	update_pipes_status(t_node node, t_pipes *pipes)
{
	if (node.flgs == PIPE)
		pipes->status = MIDDLE_PIPE;
	else
		pipes->status = END_PIPE;
}

void	swap_fds(t_pipes *pipes)
{
	int	tmp[2];

	tmp[0] = pipes->fd_a[0];
	tmp[1] = pipes->fd_a[1];
	pipes->fd_a[0] = pipes->fd_b[0];
	pipes->fd_a[1] = pipes->fd_b[1];
	pipes->fd_b[0] = tmp[0];
	pipes->fd_b[1] = tmp[1];
}

t_node	decide_cmd_node(t_tree *parent, t_pipes *pipes)
{
	if (pipes->status == END_PIPE)
		return (parent->right->node);
	else
		return (parent->left->node);
}
