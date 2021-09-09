#include "../incs/minishell.h"

/*static char	**set_env(char **envp)
{
	envp = malloc(sizeof(char **) + 1);
	envp[0] = malloc(sizeof(char *) + 1);
	envp[0] = getenv("PATH");
	envp[1] = NULL;
	return (envp);
}*/

static void	process_cmd(t_node node)
{
	//still need to figure out pipe stuff
	pid_t	c_pid;
	char	*envp[] = {"PATH=/bin:usr/bin", NULL};
	char	*cmd;

	//builtin commands
//	envp = NULL;
	c_pid = fork();
	if (c_pid == 0)
	{
//		envp = set_env(envp);
		if (ft_memcmp("/bin/", node.av[0], 5) == 0)
			cmd = ft_strdup(node.av[0]);
		else
			cmd = ft_strjoin("/bin/", node.av[0]);
		if (!cmd)
			;//free & exit bcs memory error
		if (execve(cmd, node.av, envp) == -1)
		{
			printf("minishell: %s: command not found\n", node.av[0]);
			free(cmd);
//			free(envp);
			exit (EXIT_FAILURE);
		}
//		free(envp);
		free(cmd);
	}
	else if (c_pid < 0)
		printf("PID<0");//error
	else
		wait(NULL);//look into this more
}

void	execute_input(t_tree *l)
{
	if (l != NULL)
	{
		execute_input(l->left);
		process_cmd(l->node);
		execute_input(l->right);
	}
}
