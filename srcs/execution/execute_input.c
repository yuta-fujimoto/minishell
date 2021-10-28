#include "../../incs/minishell.h"

static bool	minishell_error(t_redir redir)
{
	if (redir.perror)
		return (SUCCESS);
	else if (redir.new_in == SIGINT_CALL)
		return (FAILURE);
	else
		ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

bool	execute_input(t_tree *l, t_set *set)
{
	t_redir	redir;

	redir.perror = false;
	redir.new_in = 0;
	if (!l || set->exit_done)
		return (SUCCESS);
	if (l->node.flgs == PIPE)
	{
		if (execute_pipeline(l, set, &redir) == FAILURE)
			return (minishell_error(redir));
	}
	else
	{
		execute_input(l->left, set);
		if (l->node.av && !set->exit_done)
		{
			if (execute_simple_cmd(l->node, set, &redir) == FAILURE)
				return (minishell_error(redir));
		}
		execute_input(l->right, set);
	}
	return (SUCCESS);
}
