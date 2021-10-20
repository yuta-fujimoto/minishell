#include "../../incs/minishell.h"

int	expansion_node_conclude(t_node *node, int rlt)
{
	if (!node)
		return (rlt);
	if (node->str_flgs)
		free(node->str_flgs);
	if (node->av)
		ft_free_str_arr(node->av);
	free(node);
	return (rlt);
}

static t_node	*finish_expansion_node(t_env **env, t_node *exp_node, int ret)
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

static int	init_exp_node(t_node **exp_node, t_node *node)
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

t_node	*expansion_node(t_node *node)
{
	bool	var_exp;
	t_env	*env;
	int		i;
	t_node	*exp_node;

	exp_node = NULL;
	env = environ_to_list();
	if (!env || init_exp_node(&exp_node, node) == FAILURE)
		return (finish_expansion_node(&env, exp_node, FAILURE));
	i = 0;
	var_exp = false;
	while (node->av[i])
	{
		if (node->str_flgs[i] == STR)
		{
			if (expansion(&exp_node->av[i], &node->av[i], env, &var_exp) == FAILURE)
				return (finish_expansion_node(&env, exp_node, FAILURE));
			else
				exp_node->str_flgs[i] = STR;
		}
		else if (copy_node(exp_node, node, i) == FAILURE)
			return (finish_expansion_node(&env, exp_node, FAILURE));
		if (var_exp)
			exp_node->str_flgs[i] = STR_VAL;
		var_exp = false;
		i++;
	}
	if (eliminate_null_node(exp_node, node) == FAILURE
		|| split_argv_by_blank(exp_node) == FAILURE)
		return (finish_expansion_node(&env, exp_node, FAILURE));
	return (finish_expansion_node(&env, exp_node, SUCCESS));
}
