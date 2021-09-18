#include "../../incs/minishell.h"

int	ft_unset(char **av)
{
	int			i;
	t_env		*env;
	extern char	**environ;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	i = 1;
	while (av[i])
	{
		delete_env(&env, av[i]);
		i++;
	}
	free_environ();
	environ = list_to_environ(env);
	ft_envclear(&env, free);
	if (!environ)
		return (FAILURE);
	return (SUCCESS);
}
