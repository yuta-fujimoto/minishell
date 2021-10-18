#include "../../incs/minishell.h"

static bool	pipe_node(t_tree *prnt, t_pipes *pps, t_set *set, t_redir *rdr)
{
	t_node	node;

	node = decide_cmd_node(prnt, pps);
	if (pps->status == MIDDLE_PIPE)
	{
		if (pipe(pps->fd_b) == -1)
		{
			pps->status = END_PIPE;
			return (pipe_exit_failure(pps));
		}
	}
	if (run_pipe_cmd(node, pps, set, rdr) == FAILURE)
		return (pipe_exit_failure(pps));
	return (SUCCESS);
}

static bool	pipe_next_node(t_tree *prnt, t_pipes *pps, t_set *set, t_redir *rdr)
{
	if (pipe_node(prnt, pps, set, rdr) == FAILURE)
		return (FAILURE);
	close_pipes(pps);
	if (pps->status == MIDDLE_PIPE)
		swap_fds(pps);
	else if (pps->status == END_PIPE)
		return (SUCCESS);
	update_pipes_status(prnt->right->node, pps);
	if (pps->status == MIDDLE_PIPE)
		return (pipe_next_node(prnt->right, pps, set, rdr));
	else
		return (pipe_next_node(prnt, pps, set, rdr));
}

bool	execute_pipeline(t_tree *parent, t_set *set, t_redir *redir)
{
	t_pipes	pipes;

	if (pipe(pipes.fd_a) == -1)
		return (FAILURE);
	pipes.status = FIRST_PIPE;
	if (pipe_next_node(parent, &pipes, set, redir) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
