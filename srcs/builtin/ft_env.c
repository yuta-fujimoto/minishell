#include "../../incs/minishell.h"

bool	ft_env(void)
{
	char		**env;
	extern char	**environ;

	env = environ;
	while (*env)
	{
		ft_putendl_fd(*env, STDOUT_FILENO);
		env++;
	}
	return (SUCCESS);
}
