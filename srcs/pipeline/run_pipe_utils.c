#include "../../incs/minishell.h"

void	update_heredocs(t_node exp_node, t_set *set)
{
	int	i;

	i = -1;
	if (has_heredoc(exp_node.av))
	{
		while (exp_node.av[++i])
		{
			if (str_equal(exp_node.av[i], "<<", 3))
				set->tmp_hdocs = set->tmp_hdocs->next;
		}
	}
}
