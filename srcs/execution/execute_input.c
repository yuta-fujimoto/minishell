#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

bool	minishell_error(t_redir *redir, int *rlt, bool no_prnt)
{	
	if (redir && redir->nofile && !g_sig_info.sys_error)
		return (SUCCESS);
	*rlt = FAILURE;
	if (g_sig_info.signal == SIGINT || no_prnt)
		return (FAILURE);
	else
		ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

bool	execute_input(t_tree *l, t_set *set, int *rlt)
{
	t_redir	redir;

	redir.nofile = false;
	redir.new_in = 0;
	if (*rlt == FAILURE)
		return (FAILURE);
	if (!l || set->exit_done)
		return (SUCCESS);
	if (l->node.flgs == PIPE)
	{
		if (execute_pipeline(l, set, &redir) == FAILURE)
			return (minishell_error(&redir, rlt, false));
	}
	else
	{
		execute_input(l->left, set, rlt);
		if (l->node.av && !set->exit_done)
		{
			if (execute_simple_cmd(l->node, set, &redir) == FAILURE)
				return (minishell_error(&redir, rlt, false));
		}
		execute_input(l->right, set, rlt);
	}
	return (*rlt);
}
