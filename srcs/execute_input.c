#include "../incs/minishell.h"

bool		execve_error(char *cmd, char *cmd_path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	free(cmd_path);
	cmd_path = NULL;
	return (EXIT_FAILURE);
}

void	free_str_arr(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		str_arr[i++] = NULL;
	}
	if (str_arr)
		free(str_arr);
	str_arr = NULL;
}

static char	*create_path(char *cmd, char **paths)
{
	char		*abs_path;
	int			i;
	struct stat	ss;

	abs_path = NULL;
	i = 0;
	while (paths[i])
	{
		abs_path = ft_strcjoin(paths[i], cmd, '/');
		if ((stat(abs_path, &ss) == 0 && (ss.st_mode & S_IXUSR)) || !abs_path
			|| !paths[i + 1])
			break ;
		free(abs_path);
		abs_path = NULL;
		i++;
	}
	free_str_arr(paths);
	return (abs_path);
}

static char	**find_paths(void)
{
	char	*path;
	char	**paths;

	path = getenv("PATH");
	paths = NULL;
	paths = ft_split(path, ':');
	return (paths);
}

char	*create_cmd_path(t_node node)
{
	char	**paths;
	char	*cmd_path;

	if (node.av[0][0] != '/')
	{
		paths = find_paths();
		if (!paths)
			return (NULL);
		cmd_path = create_path(node.av[0], paths);
	}
	else
		cmd_path = ft_strdup(node.av[0]);
	return(cmd_path);
}

bool	run_shell_cmd(t_node node, t_pipes *pipes, t_set *set)
{
	pid_t		c_pid;
	char		*cmd_path;
	extern char	**environ;

	cmd_path = NULL;
	if (!is_buildin(node.av[0]))
	{
		cmd_path = create_cmd_path(node);
		if (!cmd_path)
			return (FAILURE);
	}
	c_pid = fork();
	if (c_pid < 0)
		return (FAILURE);
	else if (c_pid == 0)
	{
		if (pipes->status)
			update_pipes(pipes);
		if (is_buildin(node.av[0]))
		{
			if (run_builtin_cmd(node.av, set) == FAILURE)
				exit (execve_error(node.av[0], cmd_path));
			exit(0);
		}
		else if (execve(cmd_path, node.av, environ) == -1)
			exit (execve_error(node.av[0], cmd_path));	
	}
	else
	{
		if (wait_options(c_pid) == FAILURE)
			return (FAILURE);
		if (cmd_path)
			free(cmd_path);
	}
	return (SUCCESS);
}

bool	execute_input(t_tree *l, t_set *set)
{
	t_pipes	pipes;

	pipes.status = 0;
	if (l && l->node.flgs == PIPE)
	{
		if (ms_pipe(l, &pipes, set) == FAILURE)
			return (minishell_error());
	}
	else if (l)
	{
		execute_input(l->left, set);
		if (l->node.av)
		{
			if (is_buildin(l->node.av[0]))
			{
				if (run_builtin_cmd(l->node.av, set) == FAILURE)
					return (minishell_error());
			}
			else if (run_shell_cmd(l->node, &pipes, set) == FAILURE)
				return (minishell_error());
		}
		execute_input(l->right, set);
	}
	return (SUCCESS);
}
