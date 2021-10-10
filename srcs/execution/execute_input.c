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
	if (!l)
		return (SUCCESS);
	else if (l->node.flgs == PIPE)
	{
		if (execute_pipeline(l, set) == FAILURE)
			return (minishell_error(redir));
	}
	else
	{
		execute_input(l->left, set);
		if (l->node.av)
		{
			if (execute_simple_cmd(l->node.av, set, &redir) == FAILURE)
				return (minishell_error(redir));
		}
		execute_input(l->right, set);
	}
	return (SUCCESS);
}
