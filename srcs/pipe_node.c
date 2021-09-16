#include "../incs/minishell.h"

bool		minishell_error(void)
{
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

static bool	pipe_exit_failure(int *fd)
{
	if (fd)
	{
		if (fd[0])
			close(fd[0]);
		if (fd[1])
			close(fd[1]);
	}
	return (minishell_error());
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

bool	wait_options(pid_t pid)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	if (!WIFEXITED(wstatus))
		return (minishell_error());
	return (SUCCESS);
}

static bool	process_child(int *fd, char **environ, t_node node, int option)
{
	pid_t	pid;
	char	*cmd_path;

	cmd_path = create_cmd_path(node);
	if (!cmd_path)
		return (minishell_error());
	pid = fork();
	if (pid < 0)
		return (minishell_error());
	else if (pid > 0)
	{
		if (wait_options(pid) == FAILURE)
			return (minishell_error());
		free(cmd_path);
	}
	else
	{
		update_pipe(fd, option);
		if (execve(cmd_path, node.av, environ) == -1)
			exit (execve_error(node.av[0], cmd_path));
	}
	return (SUCCESS);
}

bool	pipe_node(t_node l, t_node r)
{
	int			fd[2];
	extern char	**environ;

	if (pipe(fd) == -1)
		return (minishell_error());
	//if builtin do builtin, else process child l
	if (process_child(fd, environ, l, 1) == FAILURE)
		return (pipe_exit_failure(fd));
	close(fd[1]);
	//if builtin do builtin, else process child r
	if (process_child(fd, environ, r, 0) == FAILURE)
		return (pipe_exit_failure(fd));
	close(fd[0]);
	return (SUCCESS);
}
