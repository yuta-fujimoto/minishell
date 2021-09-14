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
		if ((stat(abs_path, &ss) == 0 && (ss.st_mode & S_IXUSR)) || !abs_path
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

static bool	process_cmd(t_node node)
{
	pid_t		c_pid;
	char		*cmd_path;
	extern char	**environ;	
	int			wstatus;

	cmd_path = create_cmd_path(node);
	if (!cmd_path)
		return (FAILURE);
	c_pid = fork();
	if (c_pid == 0)
	{
		if (execve(cmd_path, node.av, environ) == -1)
		{
			printf("minishell: %s: command not found\n", node.av[0]);
			free(cmd_path);
			exit (EXIT_FAILURE);
		}
		free(cmd_path);
	}
	else if (c_pid < 0)
	{
		ft_putendl_fd("Child process could not be created", STDERR_FILENO);
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
	if (l != NULL)
	{
		if (l->node.flgs == PIPE)
		{
			if (pipe_node(l->left->node, l->right->node) == FAILURE)
				return (FAILURE);
		}
		else
		{
			execute_input(l->left);
			if (l->node.av)
			{
//				if (l->node.av[0] == a builtin cmd)
//					run_builtin_cmd;
//				else
				if (process_cmd(l->node) == FAILURE)
					return (FAILURE);
			}
			execute_input(l->right);
		}
	}
	return (SUCCESS);
}
