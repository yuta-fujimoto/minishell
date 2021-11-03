#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

bool	ft_env(void)
{
	char		**env;
	extern char	**environ;

	g_sig_info.exit_status = EXIT_SUCCESS;
	env = environ;
	while (*env)
	{
		if (ft_strchr(*env, '='))
			ft_putendl_fd(*env, STDOUT_FILENO);
		env++;
	}
	return (SUCCESS);
}
