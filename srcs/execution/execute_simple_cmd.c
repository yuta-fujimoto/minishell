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

static char	**create_cmd(t_node *node, t_redir *redir, bool *touch)
{
	if (has_redirection(node))
		return (ms_redirection(node, redir, touch));
	else
		return (node->av);
}

bool	execute_simple_cmd(t_node node, t_set *set, t_redir *redir)
{
	int		rlt;
	bool	touch;
	char	**cmd;
	t_node	*exp_node;

	touch = false;
	rlt = SUCCESS;
	exp_node = expansion_node(&node);
	if (!exp_node)
		return (expansion_node_conclude(NULL, FAILURE));
	if (!exp_node->av)
		return (expansion_node_conclude(exp_node, SUCCESS));
	cmd = create_cmd(exp_node, redir, &touch);
	if (!cmd && !touch)
		return (end_redirection(NULL, redir, FAILURE));
	else if (!touch)
	{
		if (is_buildin(cmd[0]))
			rlt = run_builtin_cmd(cmd, set);
		else
			rlt = run_gnu_cmd(cmd);
	}
	if (has_redirection(exp_node))
		rlt = end_redirection(cmd, redir, rlt);
	return (expansion_node_conclude(exp_node, rlt));
}
