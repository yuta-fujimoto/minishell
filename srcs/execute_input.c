#include "../incs/minishell.h"

static void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i++]);
		paths[i] = NULL;
	}
	free(paths);
	paths = NULL;
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
		if ((stat(abs_path, &ss) == 0 && ss.st_mode & S_IXUSR) || !abs_path
			|| !paths[i + 1])
			break ;
		free(abs_path);
		abs_path = NULL;
		i++;
	}
	free_paths(paths);
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

static bool	process_cmd(t_node node)
{
	pid_t		c_pid;
	char		*cmd;
	extern char	**environ;
	char		**paths;
	int			wstatus;

	if (node.av[0][0] != '/')
	{
		paths = find_paths();
		if (!paths)
			return (FAILURE);
		cmd = create_path(node.av[0], paths);
	}
	else
		cmd = ft_strdup(node.av[0]);
	if (!cmd)
		return (FAILURE);
	c_pid = fork();
	if (c_pid == 0)
	{
		if (execve(cmd, node.av, environ) == -1)
		{
			printf("minishell: %s: command not found\n", node.av[0]);
			free(cmd);
			exit (EXIT_FAILURE);
		}
		free(cmd);
	}
	else if (c_pid < 0)
	{
		dprintf(STDERR_FILENO, "Child process could not be created\n");
		return (FAILURE);
	}
	else
		wait(&wstatus);//look into other wait options need waitpid?
	if (!WIFEXITED(wstatus))
		return (FAILURE);
	return (SUCCESS);
}

bool	execute_input(t_tree *l)
{
	int	ret;

	ret = 0;
	if (l != NULL)
	{
		execute_input(l->left);
		if (l->node.av)
		{
//			if (l->node.av[0] == a builtin cmd)
//				run_builtin_cmd;
//			else
			if (process_cmd(l->node) == FAILURE)
				return (FAILURE);
		}
		execute_input(l->right);
	}
	return (SUCCESS);
}
