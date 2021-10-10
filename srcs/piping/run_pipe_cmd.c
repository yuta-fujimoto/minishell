#include "../../incs/minishell.h"

bool	free_cmd_path(char *cmd_path)
{
	free(cmd_path);
	cmd_path = NULL;
	return (FAILURE);
}

static void	update_pipes(t_pipes *pipes)
{
	if (pipes->status == FIRST_PIPE)
	{
		close(pipes->fd_a[0]);
		dup2(pipes->fd_a[1], STDOUT_FILENO);
		close(pipes->fd_a[1]);
	}
	else if (pipes->status == MIDDLE_PIPE)
	{
		dup2(pipes->fd_a[0], STDIN_FILENO);
		close(pipes->fd_a[0]);
		close(pipes->fd_b[0]);
		dup2(pipes->fd_b[1], STDOUT_FILENO);
	}
	else
	{
		close(pipes->fd_a[1]);
		dup2(pipes->fd_a[0], STDIN_FILENO);
		close(pipes->fd_a[0]);
	}
}

static int	get_cmd_path(t_node node, char **cmd_path)
{
	if (is_buildin(node.av[0]))
		*cmd_path = NULL;
	else
	{
		*cmd_path = create_cmd_path(node);
		if (!*cmd_path)
			return (FAILURE);
	}
	return (SUCCESS);
}

static void	run_child(t_node node, t_pipes *pipes, t_set *set, char *cmd_path)
{
	extern char	**environ;

	update_pipes(pipes);
	if (is_buildin(node.av[0]))
	{
		if (run_builtin_cmd(node.av, set) == FAILURE)
			exit(exec_cmd_error(node.av[0], cmd_path));
		exit(EXIT_SUCCESS);
	}
	else if (execve(cmd_path, node.av, environ) == -1)
		exit(exec_cmd_error(node.av[0], cmd_path));
}

bool	run_pipe_cmd(t_node node, t_pipes *pipes, t_set *set)
{
	pid_t		c_pid;
	char		*cmd_path;

	if (expansion_node(&node) == FAILURE)
		return (FAILURE);
	if (!node.av)
		return (SUCCESS);
	if (get_cmd_path(node, &cmd_path) == FAILURE)
		return (FAILURE);
	c_pid = fork();
	if (c_pid < 0)
		return (free_cmd_path(cmd_path));
	else if (c_pid == 0)
		run_child(node, pipes, set, cmd_path);
	else
	{
		if (wait_options(c_pid) == FAILURE)
			return (free_cmd_path(cmd_path));
		if (cmd_path)
			free(cmd_path);
	}
	expansion_node_conclude(&node);
	return (SUCCESS);
}
