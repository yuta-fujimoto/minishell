#include "../../incs/minishell.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern t_sig_info	g_sig_info;

static int	check_file(char *cmd_path)
{
	struct stat	ss;

	stat(cmd_path, &ss);
	if ((ss.st_mode & S_IXUSR) && (ss.st_mode & S_IRUSR))
	{
		free(cmd_path);
		return (EXIT_SUCCESS);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd_path, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	}
	free(cmd_path);
	return (126);
}

int	exec_cmd_error(char *cmd, char *cmd_path, bool child_failure)
{
	if (child_failure || !is_acceptable_error(errno) || errno == ENOEXEC)
	{	
		if (errno == ENOEXEC)
			return (check_file(cmd_path));
		free(cmd_path);
		return (CHILD_FAILURE);
	}
	if (errno == EACCES)
		cmd = cmd_path;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
	free(cmd_path);
	if (errno == EACCES)
		return (126);
	else if (errno == ENOENT)
		return (127);
	return (EXIT_FAILURE);
}

bool	wait_options(pid_t pid, bool pipeline)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	if (((!WIFEXITED(wstatus) && !pipeline)
			|| WEXITSTATUS(wstatus) == CHILD_FAILURE)
		&& WEXITSTATUS(wstatus) != EXIT_SUCCESS)
		return (false);
	if (g_sig_info.exit_status == 130)
		return (true);
	if (g_sig_info.exit_status == 131)
		return (true);
	else
		g_sig_info.exit_status = WEXITSTATUS(wstatus);
	return (true);
}
