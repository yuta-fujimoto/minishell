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

static bool	conclude_pipeline(t_pidlist *pidlst)
{
//	t_pidlist	*tmp;//change later once working

//	tmp = *pidlst;
	while (pidlst)
	{
		if (!wait_options(pidlst->pid))
			return (false);
		pidlst = pidlst->next;
	}
	return (true);
}

static bool	free_pidlst(t_pidlist **pidlst, int rlt)
{
	ft_pidlstclear(pidlst);
	return (rlt);
}

bool	execute_pipeline(t_tree *parent, t_set *set, t_redir *redir)
{
	t_pipes	pipes;

	pipes.status = FIRST_PIPE;
	pipes.pidlst = NULL;
	if (pipe(pipes.fd_a) == -1)
		return (FAILURE);
	if (pipe_next_node(parent, &pipes, set, redir) == FAILURE)
		return (free_pidlst(&pipes.pidlst, FAILURE));
	if (!conclude_pipeline(pipes.pidlst))
		return (free_pidlst(&pipes.pidlst, FAILURE));
	return (free_pidlst(&pipes.pidlst, SUCCESS));
}
