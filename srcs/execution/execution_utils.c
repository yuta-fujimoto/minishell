#include "../../incs/minishell.h"

bool	exec_cmd_error(char *cmd, char *cmd_path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	free(cmd_path);
	cmd_path = NULL;
	return (EXIT_FAILURE);
}

bool	wait_options(pid_t pid)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	if (!WIFEXITED(wstatus) || WEXISTATUS(wstatus) != EXIT_SUCCESS)
		return (false);
	return (true);
}
