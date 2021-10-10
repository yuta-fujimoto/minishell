#include "../../incs/minishell.h"

static bool	minishell_error(void)
{
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

static bool	run_gnu_cmd(t_node node)
{
	pid_t		c_pid;
	char		*cmd_path;
	extern char	**environ;

	cmd_path = create_cmd_path(node);
	if (!cmd_path)
		return (FAILURE);
	c_pid = fork();
	if (c_pid < 0)
		return (free_cmd_path(cmd_path));
	else if (c_pid == 0)
	{
		if (execve(cmd_path, node.av, environ) == -1)
			exit(exec_cmd_error(node.av[0], cmd_path));
	}
	else
	{
		if (wait_options(c_pid) == FAILURE)
			return (free_cmd_path(cmd_path));
		free(cmd_path);
	}
	return (SUCCESS);
}

static bool	execute_cmd(t_node node, t_set *set)
{
	int	rlt;

	if (expansion_node(&node) == FAILURE)
		return (FAILURE);
	if (node.av == NULL)
		return (SUCCESS);
	if (is_buildin(node.av[0]))
		rlt = run_builtin_cmd(node.av, set);
	else
		rlt = run_gnu_cmd(node);
	expansion_node_conclude(&node);
	return (rlt);
}

bool	execute_input(t_tree *l, t_set *set)
{
	int		rlt;

	rlt = SUCCESS;
	if (!l)
		return (SUCCESS);
	else if (l->node.flgs == PIPE)
		rlt = execute_pipe(l, set);
	else
	{
		execute_input(l->left, set);
		if (l->node.av)
			rlt = execute_cmd(l->node, set);
		execute_input(l->right, set);
	}
	if (rlt == FAILURE)
		return (minishell_error());
	return (rlt);
}
