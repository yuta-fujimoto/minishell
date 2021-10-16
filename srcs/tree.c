#include "../incs/minishell.h"

extern int	fd;

t_tree	*new_tree(int flgs, t_tree *left, t_tree *right)
{
	t_tree	*tree;
	t_node	node;

	tree = ft_calloc(1, sizeof(t_tree));
	tree->left = left;
	tree->right = right;
	node.ac = 0;
	node.av = 0;
	node.flgs = flgs;
	node.str_flgs = NULL;
	tree->node = node;
	return (tree);
}

t_tree	*new_tree_cmd(t_node node)
{
	t_tree	*tree;

	tree = ft_calloc(1, sizeof(t_tree));
	tree->node = node;
	return (tree);
}

void	process_item(t_node node, int h)
{
	int	i;

	i = -1;
	while (h-- > 0)
		dprintf(fd, "\t");
	dprintf(fd, "%d %d >> ", node.flgs, node.ac);
	while (++i < node.ac)
		dprintf(fd, "[%s]", node.av[i]);
	dprintf(fd, "\n");
}

void	traverse_tree(t_tree *l, int h)
{
	if (l != NULL)
	{
		traverse_tree(l->left, h + 1);
		process_item(l->node, h);
		traverse_tree(l->right, h + 1);
	}
}

void	free_tree(t_tree *l)
{
	if (l != NULL)
	{
		free_tree(l->left);
		free_tree(l->right);
		if (l->node.flgs == STR && l->node.av)
			free(l->node.av);
		if (l->node.str_flgs)
			free(l->node.str_flgs);
		free(l);
	}
}
