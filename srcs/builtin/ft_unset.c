#include "../../incs/minishell.h"

int	ft_unset(char **av, t_env **env)
{
	int i;

	if (!av)
		return (SUCCESS);
	i = 0;
	while (av[i])
	{
		delete_env(env, av[i]);
		i++;
	}
	return (SUCCESS);
}
