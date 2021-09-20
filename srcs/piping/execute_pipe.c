#include "../../incs/minishell.h"

static bool	pipe_node(t_tree *parent, t_pipes *pipes, t_set *set)
{
	t_node	node;

	node = decide_cmd_node(parent, pipes);
	if (pipes->status == MIDDLE_PIPE)
	{
		if (pipe(pipes->fd_b) == -1)
		{
			pipes->status = END_PIPE;
			return (pipe_exit_failure(pipes));
		}
	}
	if (run_pipe_cmd(node, pipes, set) == FAILURE)
		return (pipe_exit_failure(pipes));
	return (SUCCESS);
}

static bool	pipe_next_node(t_tree *parent, t_pipes *pipes, t_set *set)
{
	if (pipe_node(parent, pipes, set) == FAILURE)
		return (FAILURE);
	close_pipes(pipes);
	if (pipes->status == MIDDLE_PIPE)
		swap_fds(pipes);
	else if (pipes->status == END_PIPE)
		return (SUCCESS);
	update_pipes_status(parent->right->node, pipes);
	if (pipes->status == MIDDLE_PIPE)
		return (pipe_next_node(parent->right, pipes, set));
	else
		return (pipe_next_node(parent, pipes, set));
}

bool	execute_pipe(t_tree *parent, t_set *set)
{
	t_pipes	pipes;

	if (pipe(pipes.fd_a) == -1)
		return (FAILURE);
	pipes.status = FIRST_PIPE;
	if (pipe_next_node(parent, &pipes, set) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
