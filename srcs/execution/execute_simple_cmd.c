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

bool	execute_simple_cmd(t_node node, t_set *set, t_redir *redir)
{
	int		rlt;
	bool	touch;
	char	**old_av;

	touch = false;
	rlt = SUCCESS;
	old_av = node.av;
	node.av = create_cmd(node.av, redir, &touch);
	if (!node.av && !touch)
		return (end_redirection(NULL, redir, FAILURE));
	node.ac = ft_str_arr_len(node.av);
	if (expansion_node(&node) == FAILURE)
		return (end_redirection(NULL, redir, FAILURE));
	else if (!touch && node.av)
	{
		if (is_buildin(node.av[0]))
			rlt = run_builtin_cmd(node.av, set);
		else
			rlt = run_gnu_cmd(node.av);
	}
	if (has_redirection(old_av))
		rlt = end_redirection(node.av, redir, rlt);
	else
		expansion_node_conclude(&node);
	return (rlt);
}
