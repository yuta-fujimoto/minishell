#include "../incs/minishell.h"
#include <sys/stat.h>

static void	free_paths(char **paths)
{
	while (*paths)
	{
		free(*paths++);
		*paths = NULL;	
	}
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
		if  (stat(abs_path, &ss) == 0 && ss.st_mode & S_IXUSR)
			break ;
		free(abs_path);
		abs_path = NULL;
		i++;
	}
	free_paths(paths);
	free(paths);
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

static void	process_cmd(t_node node)
{
	pid_t		c_pid;
	char		*cmd;
	extern char	**environ;
	char		**paths;
	
	if (node.av[0][0] != '/')
	{
		paths = find_paths();
	//figure out what to do in case of NULL (ie, '/' not found or mllc err)
		cmd = create_path(node.av[0], paths);
	}
	else
		cmd = ft_strdup(node.av[0]);
	//figure out what to do in case of NULL (ie, '/' not found or mllc err)
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
		printf("PID<0");//figure out what to do in case of pid error
	else
		wait(NULL);//look into other wait options
}

void	execute_input(t_tree *l)
{
	if (l != NULL)
	{
		execute_input(l->left);
		if (l->node.av)
		{
//			if (l->node.av[0] == a builtin cmd)
//				run_builtin_cmd;
//			else
			process_cmd(l->node);
		}
		execute_input(l->right);
	}
}
