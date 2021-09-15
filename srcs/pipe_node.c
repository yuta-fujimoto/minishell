#include "../incs/minishell.h"

static bool	pipe_exit_failure(int *fd, char *message)
{
	ft_putendl_fd(message, STDERR_FILENO);
	if (fd)
	{
		if (fd[0])
			close(fd[0]);
		if (fd[1])
			close(fd[1]);
	}
	return (false);
}

static void	update_pipe(int *fd, int option)
{
	if (option == 1)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
}

static bool	wait_options(pid_t pid)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	if (!WIFEXITED(wstatus))
		return (FAILURE);
	return (SUCCESS);
}

static bool	process_child(int *fd, char **environ, t_node node, int option)
{
	pid_t	pid;
	char	*cmd_path;

	cmd_path = create_cmd_path(node);
	if (!cmd_path)
		return (FAILURE);
	pid = fork();
	if (pid < 0)
		return (FAILURE);
	else if (pid > 0)
	{
		if (wait_options(pid) == FAILURE)
			return (FAILURE);
	}
	else
	{
		update_pipe(fd, option);
		if (execve(cmd_path, node.av, environ) == -1)
		{
			printf("minishell: %s: command not found\n", node.av[0]);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}		
		free(cmd_path);
	}
	return (SUCCESS);
}

bool	pipe_node(t_node l, t_node r)
{
	int			fd[2];
	extern char	**environ;

	if (pipe(fd) == -1)
		return (pipe_exit_failure(NULL, "minishell pipe error1"));
	//if builtin do builtin, else process child l
	if (process_child(fd, environ, l, 1) == FAILURE)
		return (pipe_exit_failure(fd, "minishell pipe error2"));
	close(fd[1]);
	//if builtin do builtin, else process child r
	if (process_child(fd, environ, r, 0) == FAILURE)
		return (pipe_exit_failure(fd, "minishell pipe error3"));
	close(fd[0]);
	return (SUCCESS);
}
