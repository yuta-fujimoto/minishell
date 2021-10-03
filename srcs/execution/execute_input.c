#include "../../incs/minishell.h"

static bool	minishell_error(void)
{
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

bool	execute_input(t_tree *l, t_set *set)
{
	if (!l)
		return (SUCCESS);
	else if (l->node.flgs == PIPE)
	{
		if (execute_pipeline(l, set) == FAILURE)
			return (minishell_error());
	}
	else
	{
		execute_input(l->left, set);
		if (l->node.av)
		{
			if (execute_simple_cmd(l->node.av, set) == FAILURE)
				return (minishell_error());
		}
		execute_input(l->right, set);
	}
	return (SUCCESS);
}
