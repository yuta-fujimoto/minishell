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
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (true);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (true);
	return (false);
}

int	run_builtin_cmd(char **av, t_set *set)
{
	int	rlt;

	if (ft_strncmp(av[0], "export", 7) == 0)
		rlt = ft_export(av);
	if (ft_strncmp(av[0], "env", 4) == 0)
		rlt = ft_env();
	if (ft_strncmp(av[0], "unset", 6) == 0)
		rlt = ft_unset(av);
	if (ft_strncmp(av[0], "pwd", 4) == 0)
		rlt = ft_pwd();
	if (ft_strncmp(av[0], "exit", 5) == 0)
		ft_exit(av, set);
	if (ft_strncmp(av[0], "cd", 3) == 0)
		rlt = ft_cd(av);
	return (rlt);
}
