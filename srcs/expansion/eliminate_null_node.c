#include "../../incs/minishell.h"

static int	get_new_av_size(t_node *node)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (i < node->ac)
	{
		if (node->av[i] != NULL)
			size++;
		i++;
	}
	return (size);
}

int	eliminate_null_node(t_node *node)
{
	char	**new_av;
	int		new_size;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_size = get_new_av_size(node);
	if (new_size == 0)
	{
		node->av = NULL;
		return (SUCCESS);
	}
	new_av = ft_calloc(sizeof(char *), new_size + 1);
	if (!new_av)
		return (FAILURE);
	while (j < new_size)
	{
		if (node->av[i] != NULL)
			new_av[j++] = node->av[i];
		i++;
	}
	node->av = new_av;
	node->ac = new_size;
	return (SUCCESS);
}
