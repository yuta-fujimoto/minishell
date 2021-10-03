#include "../../incs/minishell.h"

static bool	run_gnu_cmd(char **cmd)
{
	pid_t		c_pid;
	char		*cmd_path;
	extern char	**environ;

	cmd_path = create_cmd_path(cmd);
	if (!cmd_path)
		return (FAILURE);
	c_pid = fork();
	if (c_pid == SYS_ERROR)
		return (free_cmd_path(cmd_path));
	else if (c_pid == 0)
	{
		if (execve(cmd_path, cmd, environ) == SYS_ERROR)
			exit(exec_cmd_error(cmd[0], cmd_path));
	}
	else
	{
		if (!wait_options(c_pid))
			return (free_cmd_path(cmd_path));
		free(cmd_path);
	}
	return (SUCCESS);
}

static char	**create_cmd(char **av, t_redir *redir, bool *touch)
{
	if (has_redirection(av))
		return (ms_redirection(av, redir, touch));
	else
		return (av);
}

bool	execute_simple_cmd(char **av, t_set *set)
{
	int		rlt;
	char	**cmd;
	t_redir	redir;
	bool	touch;

	touch = false;
	rlt = SUCCESS;
	cmd = create_cmd(av, &redir, &touch);
	if (!cmd && !touch)
		return (end_redirection(NULL, &redir, FAILURE));
	else if (!touch)
	{	
		if (is_buildin(cmd[0]))
			rlt = run_builtin_cmd(cmd, set);
		else
			rlt = run_gnu_cmd(cmd);
	}
	if (has_redirection(av))
		rlt = end_redirection(cmd, &redir, rlt);
	return (rlt);
}
