#include "../../incs/minishell.h"

extern t_sig_info g_sig_info;

bool	minishell_error(t_redir *redir, int *rlt)
{	
	if (redir && redir->perror)
		return (SUCCESS);	
	if (g_sig_info.signal == SIGINT)
		;	
	else
		ft_putendl_fd("minishell:error", STDERR_FILENO);
	*rlt = FAILURE;
	return (FAILURE);
}

bool	execute_input(t_tree *l, t_set *set, int *rlt)
{
	t_redir	redir;

	redir.perror = false;
	redir.new_in = 0;
	if (*rlt == FAILURE)
		return (FAILURE);
	if (!l)
		return (SUCCESS);
	else if (l->node.flgs == PIPE)
	{
		if (execute_pipeline(l, set, &redir) == FAILURE)
			return (minishell_error(&redir, rlt));
	}
	else
	{
		execute_input(l->left, set, rlt);
		if (l->node.av)
		{
			if (execute_simple_cmd(l->node, set, &redir) == FAILURE)
				return (minishell_error(&redir, rlt));
		}
		execute_input(l->right, set, rlt);
	}
	return (rlt);
}
