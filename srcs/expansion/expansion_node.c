#include "../../incs/minishell.h"

void	expansion_node_conclude(t_node *node)
{
	int	i;

	i = 0;
	if (!node->av)
		return ;
	while (node->av[i])
	{
		free(node->av[i]);
		i++;
	}
	free(node->av);
}

int	expansion_node(t_node *node)
{
	bool	var_exp;
	t_env	*env;
	int		i;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	i = 0;
	var_exp = false;
	while (node->av[i])
	{
		if (node->str_flgs[i] == STR
			&& expansion(&node->av[i], env, &var_exp) == FAILURE)
			return (expansion_conclude(&env, NULL, FAILURE));
		if (var_exp)
			node->str_flgs[i] = STR_VAL;
		i++;
	}
	if (eliminate_null_node(node) == FAILURE
		|| split_argv_by_blank(node) == FAILURE)
		return (expansion_conclude(&env, NULL, FAILURE));
	if (list_to_environ(env) == FAILURE)
		return (expansion_conclude(&env, NULL, FAILURE));
	return (expansion_conclude(&env, NULL, SUCCESS));
}
