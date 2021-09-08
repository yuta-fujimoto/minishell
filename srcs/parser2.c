#include "../incs/minishell.h"

int	consume(int flgs, t_list **lst)
{
	if (!lst || !*lst || flgs != (*lst)->flags)
		return (false);
	*lst = (*lst)->next;
	return (true);
}

bool	get_new_node(t_list *lst, t_node *node)
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
		return (false);
	(*node).str_flgs = ft_calloc((*node).ac + 1, sizeof(int));
	if (!(*node).str_flgs)
	{
		free((*node).av);
		return (false);
	}
	return (true);
}

t_tree	*command(t_list **lst)
{
	t_node	node;
	int		i;

	if (!lst || !*lst)
		return (NULL);
	if (!get_new_node(*lst, &node))
		return (NULL);
	node.flgs = (*lst)->flags;
	i = -1;
	while (node.ac > ++i)
	{
		(node.av)[i] = (*lst)->word;
		(node.str_flgs)[i] = (*lst)->flags;
		*lst = (*lst)->next;
	}
	return (new_tree_cmd(node));
}
