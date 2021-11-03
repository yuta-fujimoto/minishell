#include "../../incs/minishell.h"
#include <unistd.h>

extern t_sig_info	g_sig_info;

bool	free_cmd_path(char *cmd_path)
{
	free(cmd_path);
	cmd_path = NULL;
	return (FAILURE);
}

int	command_not_found(char *cmd, bool path_error)
{
	if (path_error)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	g_sig_info.exit_status = 127;
	return (SUCCESS);
}
