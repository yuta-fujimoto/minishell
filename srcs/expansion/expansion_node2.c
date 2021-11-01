#include "../../incs/minishell.h"

t_node	*finish_expansion_node(t_env **env, t_node *exp_node, int ret)
{
	if (env)
		ft_envclear(env, free);
	if (ret == FAILURE)
	{
		if (!exp_node)
			return (NULL);
		if (exp_node->av)
			free(exp_node->av);
		if (exp_node->str_flgs)
			free(exp_node->str_flgs);
		free(exp_node);
		return (NULL);
	}
	return (exp_node);
}

int	init_exp_node(t_node **exp_node, t_node *node)
{
	int	i;

	i = 0;
	*exp_node = malloc(sizeof(t_node));
	if (!*exp_node)
		return (FAILURE);
	(*exp_node)->ac = node->ac;
	(*exp_node)->flgs = node->flgs;
	(*exp_node)->str_flgs = ft_calloc(sizeof(int), node->ac + 1);
	if (!(*exp_node)->str_flgs)
		return (FAILURE);
	while (node->str_flgs[i])
	{
		(*exp_node)->str_flgs[i] = node->str_flgs[i];
		i++;
	}
	(*exp_node)->av = ft_calloc(sizeof(char *), node->ac + 1);
	if (!(*exp_node)->av)
		return (FAILURE);
	return (SUCCESS);
}

int	copy_node(t_node *exp_node, t_node *node, int i)
{
	exp_node->av[i] = ft_strdup(node->av[i]);
	if (!exp_node)
		return (FAILURE);
	exp_node->str_flgs[i] = node->str_flgs[i];
	return (SUCCESS);
}
