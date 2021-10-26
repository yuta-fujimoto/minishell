#include "../incs/minishell.h"

extern t_sig_info	g_sig_info;

static t_tree	*pipeline(t_list **lst)
{
	t_tree	*tree;

	if (!lst || !*lst)
		return (NULL);
	tree = command(lst);
	if (!tree)
		return (NULL);
	if (consume(PIPE, lst))
		return (new_tree(PIPE, tree, pipeline(lst)));
	return (tree);
}

static t_tree	*list(t_list **lst)
{
	t_tree	*tree;

	if (!lst || !*lst)
		return (NULL);
	tree = pipeline(lst);
	if (!tree)
		return (NULL);
	if (consume(SCOLON, lst))
		return (new_tree(SCOLON, tree, list(lst)));
	return (tree);
}

bool	parser(t_set *set)
{
	t_list	*lst;

	lst = set->lst;
	if (!set->lst)
	{
		set->tree = NULL;
		return (true);
	}
	set->tree = list(&lst);
	if (!set->tree)
		parser_error(set);
	if (syntax_error(set->tree))
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		g_sig_info.exit_status = 258;
		return (false);
	}
	return (true);
}
