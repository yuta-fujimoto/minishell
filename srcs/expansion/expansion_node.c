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

static int	expansion_single_node(t_node *exp_node,
		t_node *node, int i, t_env *env)
{
	bool	var_exp;

	var_exp = false;
	if (node->str_flgs[i] == STR)
	{
		if (expansion(&exp_node->av[i],
				&node->av[i], env, &var_exp) == FAILURE)
			return (FAILURE);
		else
			exp_node->str_flgs[i] = STR;
	}
	else if (copy_node(exp_node, node, i) == FAILURE)
		return (FAILURE);
	if (var_exp)
		exp_node->str_flgs[i] = STR_VAL;
	return (SUCCESS);
}

t_node	*expansion_node(t_node *node)
{
	t_env	*env;
	int		i;
	t_node	*exp_node;

	exp_node = NULL;
	env = environ_to_list();
	if (!env || init_exp_node(&exp_node, node) == FAILURE)
		return (finish_expansion_node(&env, exp_node, FAILURE));
	i = 0;
	while (node->av[i])
	{
		if (expansion_single_node(exp_node, node, i, env) == FAILURE)
			return (finish_expansion_node(&env, exp_node, FAILURE));
		i++;
	}
	if (eliminate_null_node(exp_node, node) == FAILURE
		|| split_argv_by_blank(exp_node) == FAILURE)
		return (finish_expansion_node(&env, exp_node, FAILURE));
	return (finish_expansion_node(&env, exp_node, SUCCESS));
}
