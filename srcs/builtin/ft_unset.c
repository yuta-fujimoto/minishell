#include "../../incs/minishell.h"

int	ft_unset(char **av)
{
	int			i;
	t_env		*env;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	i = 1;
	while (av[i])
	{
		delete_env(&env, av[i]);
		i++;
	}
	if (list_to_environ(env) == FAILURE)
	{
		ft_envclear(&env, free);
		return (FAILURE);
	}
	ft_envclear(&env, free);
	return (SUCCESS);
}
