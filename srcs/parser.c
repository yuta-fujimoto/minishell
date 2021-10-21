#include "../incs/minishell.h"

t_tree	*pipeline(t_list **lst)
{
	t_tree	*tree;

	if (!lst || !*lst)
		return (NULL);
	tree = command(lst);
	if (consume(PIPE, lst))
		return (new_tree(PIPE, tree, pipeline(lst)));
	return (tree);
}

t_tree	*list(t_list **lst)
{
	t_tree	*tree;

	if (!lst || !*lst)
		return (NULL);
	tree = pipeline(lst);
	if (consume(SCOLON, lst))
		return (new_tree(SCOLON, tree, list(lst)));
	return (tree);
}

t_tree	*parser(t_list *lst)
{
	t_tree	*tree;

	tree = list(&lst);
	traverse_tree(tree, 0);
	if (syntax_error(tree))
		ft_putendl_fd("syntax error", STDERR_FILENO);
	return (tree);
}
