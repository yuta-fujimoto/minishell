#include "../incs/minishell.h"

bool	syntax_error_redirect(t_node node)
{
	int	i;
	int	sf;

	i = -1;
	while (++i < node.ac)
	{
		sf = node.str_flgs[i];
		if ((sf >= RDIR && sf <= LLDIR) && node.str_flgs[i + 1] != STR)
			return (true);
	}
	return (false);
}

bool	syntax_error(t_tree *tree)
{
	if (!tree)
		return (false);
	if (syntax_error(tree->left) || syntax_error(tree->right))
		return (true);
	if (tree->node.flgs == PIPE && (!tree->left || !tree->right))
		return (true);
	if (tree->node.flgs == SCOLON && !tree->left)
		return (true);
	if (tree->node.flgs == STR && syntax_error_redirect(tree->node))
		return (true);
	return (false);
}
