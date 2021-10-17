#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

bool	is_nflg(char *s)
{
	int	i;

	i = 1;
	if (!s || s[0] != '-')
		return (false);
	while (s[i])
	{
		if (s[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	ft_echo(char **av)
{
	bool	nflag;
	int		i;

	i = 1;
	nflag = false;
	g_sig_info.exit_status = EXIT_SUCCESS;
	while (is_nflg(av[i]))
	{
		nflag = true;
		i++;
	}
	while (av[i])
	{
		ft_putstr_fd(av[i], STDOUT_FILENO);
		if (av[++i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!nflag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
