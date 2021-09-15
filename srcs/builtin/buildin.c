#include "../../incs/minishell.h"

bool	is_buildin(char *cmd)
{
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (true);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (true);
	return (false);
}

int		run_builtin_cmd(char **av)
{
	t_env		*env;
	extern char	**environ;
	int			rlt;

	env = environ_to_list(environ);
	if (!env)
		return (FAILURE);
	if (ft_strncmp(av[0], "export", 7) == 0)
		rlt = ft_export(av, &env);
	if (ft_strncmp(av[0], "env", 4) == 0)
		rlt = ft_env();
	if (ft_strncmp(av[0], "unset", 6) == 0)
		rlt = ft_unset(av, &env);
	if (ft_strncmp(av[0], "pwd", 4) == 0)
		rlt = ft_pwd();
	free_environ(environ);
	environ = list_to_environ(env);
	ft_envclear(&env, free);
	return (rlt);
}
