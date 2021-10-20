#include "../../incs/minishell.h"

bool	is_buildin(char *cmd)
{
	if (str_equal(cmd, "export", 7))
		return (true);
	if (str_equal(cmd, "env", 4))
		return (true);
	if (str_equal(cmd, "unset", 6))
		return (true);
	if (str_equal(cmd, "pwd", 4))
		return (true);
	if (str_equal(cmd, "exit", 5))
		return (true);
	if (str_equal(cmd, "cd", 3))
		return (true);
	if (str_equal(cmd, "echo", 5))
		return (true);
	return (false);
}

int	run_builtin_cmd(char **av, t_set *set)
{
	int	rlt;

	if (str_equal(av[0], "export", 7))
		rlt = ft_export(av);
	if (str_equal(av[0], "env", 4))
		rlt = ft_env();
	if (str_equal(av[0], "unset", 6))
		rlt = ft_unset(av);
	if (str_equal(av[0], "pwd", 4))
		rlt = ft_pwd();
	if (str_equal(av[0], "exit", 5))
		rlt = ft_exit(av, set);
	if (str_equal(av[0], "cd", 3))
		rlt = ft_cd(av);
	if (str_equal(av[0], "echo", 5))
		rlt = ft_echo(av);
	return (rlt);
}
