#include "../incs/minishell.h"

void	parser_error(t_set *set)
{
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	ft_lstclear(&set->lst, free);
	free(set->input);
	if (!mod_termios_attr(set, false))
		exit(SYS_ERROR);
	exit(EXIT_FAILURE);
}

int	consume(int flgs, t_list **lst)
{
	if (!lst || !*lst || flgs != (*lst)->flags)
		return (false);
	*lst = (*lst)->next;
	return (true);
}

bool	get_new_node(t_list *lst, t_node *node, bool *malloc_err)
{
	(*node).ac = 0;
	while (lst && (lst->flags >= STR && lst->flags <= LLDIR))
	{
		(*node).ac++;
		lst = lst->next;
	}
	if ((*node).ac == 0)
		return (false);
	(*node).av = ft_calloc((*node).ac + 1, sizeof(char *));
	if (!(*node).av)
	{
		*malloc_err = true;
		return (false);
	}
	(*node).str_flgs = ft_calloc((*node).ac + 1, sizeof(int));
	if (!(*node).str_flgs)
	{
		*malloc_err = true;
		free((*node).av);
		return (false);
	}
	return (true);
}

t_tree	*command(t_list **lst, bool *malloc_err)
{
	t_node	node;
	int		i;

	if (!lst || !*lst || *malloc_err)
		return (NULL);
	if (!get_new_node(*lst, &node, malloc_err))
		return (NULL);
	if ((*lst)->flags >= STR && (*lst)->flags <= LLDIR)
		node.flgs = STR;
	else
		node.flgs = (*lst)->flags;
	i = -1;
	while (node.ac > ++i)
	{
		(node.av)[i] = (*lst)->word;
		(node.str_flgs)[i] = (*lst)->flags;
		*lst = (*lst)->next;
	}
	return (new_tree_cmd(node, malloc_err));
}
