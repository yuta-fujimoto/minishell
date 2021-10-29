#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

int	exec_cmd_error(char *cmd, char *cmd_path, bool malloc_failure)
{
	if (malloc_failure)
		return (CHILD_FAILURE);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
	free(cmd_path);
	cmd_path = NULL;
	if (errno == EACCES)
		return (126);
	return (EXIT_FAILURE);
}

bool	wait_options(pid_t pid, bool pipeline)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	if ((!WIFEXITED(wstatus) && !pipeline)
		|| WEXITSTATUS(wstatus) == REDIRECTION_FAILURE
		|| WEXITSTATUS(wstatus) == CHILD_FAILURE
		|| WEXITSTATUS(wstatus) == SYS_ERROR)
		return (false);
	g_sig_info.exit_status = WEXITSTATUS(wstatus);
	return (true);
}
