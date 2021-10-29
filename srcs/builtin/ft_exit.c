#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

void	exit_error(char *arg, char *msg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}

bool	ft_exit(char **av, t_set *set, bool print_exit)
{
	int	status;
	int	flg;
	
	if (print_exit)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (!av[1])
	{
		free_set(set);
		if (!mod_termios_attr(set, false))
			exit(SYS_ERROR);
		exit(g_sig_info.exit_status);
	}
	status = ft_atol(av[1], &flg) % 256;
	if (flg)
	{
		exit_error(av[1], "numeric argument required");
		free_set(set);
		if (!mod_termios_attr(set, false))
			exit(SYS_ERROR);
		exit(255);
	}
	if (av[2])
	{
		set->exit_done = true;
		exit_error(NULL, "too many arguments");
		free_set(set);
		g_sig_info.exit_status = EXIT_FAILURE;
		return (SUCCESS);
	}
	if (!mod_termios_attr(set, false))
		exit(SYS_ERROR);
	exit(status);
}
