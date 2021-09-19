#include "../../incs/minishell.h"

bool	pipe_exit_failure(int fd1, int fd2)
{
	if (fd1)
		close(fd1);
	if (fd2)
		close(fd2);
	return (FAILURE);
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

void	close_pipes(t_pipes *pipes)
{
	if (pipes->status == FIRST_PIPE)
		close(pipes->fd_a[1]);
	else if (pipes->status == MIDDLE_PIPE)
	{	
		close(pipes->fd_a[0]);
		close(pipes->fd_b[1]);
	}
	else
		close(pipes->fd_a[0]);
}

t_node	decide_cmd_node(t_tree *parent, t_pipes *pipes)
{
	if (pipes->status == END_PIPE)
		return (parent->right->node);
	else
		return (parent->left->node);
}
