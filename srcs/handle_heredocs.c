#include "../incs/minishell.h"

static bool	create_heredoc(char **av, t_set *set)
{
	int		i;
	bool	expansion_required;
	int		new_in;

	i = 0;
	expansion_required = true;
	while (!str_equal(av[i], "<<", 3))
		i++;
	new_in = open_heredoc(av[++i]);//need to expand av
	if (new_in == SYS_ERROR || new_in == SIGINT_CALL)
		return (FAILURE);
	if (!ft_intlstadd_back(&set->heredoc_lst, ft_intlstnew(new_in)))
		return (FAILURE);	
	return (SUCCESS);
}

bool	handle_heredocs(t_tree *l, t_set *set, int *rlt)
{
	if (*rlt == FAILURE)
		return (FAILURE);
	if (!l)
		return (SUCCESS);
	handle_heredocs(l->left, set, rlt);	
	if (l->node.av && has_heredoc(l->node.av))
	{
		if (create_heredoc(l->node.av, set) == FAILURE)
			return (minishell_error(NULL, rlt));
	}
	handle_heredocs(l->right, set, rlt);
	return (rlt);
}
//need to add lots and lots of error handling
