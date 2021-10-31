#include "../../incs/minishell.h"

static char	*ft_strlowcase(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 'a' - 'A';
		i++;
	}
	return (str);
}

bool	is_buildin(char *cmd)
{
	cmd = ft_strlowcase(cmd);
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

int	run_builtin_cmd(char **av, t_set *set, bool print_exit)
{
	int		rlt;
	char	*cmd;

	cmd = ft_strlowcase(av[0]);
	if (str_equal(cmd, "export", 7))
		rlt = ft_export(av);
	if (str_equal(cmd, "env", 4))
		rlt = ft_env();
	if (str_equal(cmd, "unset", 6))
		rlt = ft_unset(av);
	if (str_equal(cmd, "pwd", 4))
		rlt = ft_pwd();
	if (str_equal(cmd, "exit", 5))
		rlt = ft_exit(av, set, print_exit);
	if (str_equal(cmd, "cd", 3))
		rlt = ft_cd(av);
	if (str_equal(cmd, "echo", 5))
		rlt = ft_echo(av);
	return (rlt);
}
