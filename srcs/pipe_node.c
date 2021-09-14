#include "../incs/minishell.h"

static bool	pipe_exit_failure(int *fd, pid_t pid, char *message)
{
	ft_putendl_fd(message, STDERR_FILENO);
	if (fd)
	{
		if (fd[0])
			close(fd[0]);
		if (fd[1])
			close(fd[1]);
	}
	if (pid)
		kill(pid, SIGKILL);
	return (false);	
}

bool	pipe_node(t_node l, t_node r)
{
	int			fd[2];
	pid_t		c1_pid;
	pid_t		c2_pid;
	char		*cmd_path1;
	char		*cmd_path2;
	int			wstatus;
	int			wstatus2;
	extern char	**environ;

	/*init*/
	if (pipe(fd) == -1)
		return (pipe_exit_failure(NULL, 0, "minishell pipe error"));
	/*1st command*/
	cmd_path1 = create_cmd_path(l);
	if (!cmd_path1)
		return (pipe_exit_failure(fd, 0, "minishell fork error"));
	c1_pid = fork();
	if (c1_pid < 0)
		return (pipe_exit_failure(fd, 0, "minishell fork error"));
	else if (c1_pid > 0)
	{	
		waitpid(c1_pid, &wstatus, 0);	
		if (!WIFEXITED(wstatus))
			return (FAILURE);	
	}
	else
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (execve(cmd_path1, l.av, environ) == -1)
		{
			printf("minishell: %s: command not found\n", l.av[0]);
			free(cmd_path1);
			//exit close fds
			exit(EXIT_FAILURE);
		}	
		close(fd[1]);
		free(cmd_path1);
	}
	close(fd[1]);
	/*2nd command*/
	cmd_path2 = create_cmd_path(r);
	if (!cmd_path2)
		return (pipe_exit_failure(fd, c1_pid, "minishell fork error"));
	c2_pid = fork();
	if (c2_pid < 0)
		return (pipe_exit_failure(fd, c1_pid, "minishell fork error"));
	else if (c2_pid > 0)
	{	
		waitpid(c2_pid, &wstatus2, 0);	
		if (!WIFEXITED(wstatus2))
			return (FAILURE);
	}
	else
	{	
		close(fd[1]);	
		dup2(fd[0], STDIN_FILENO);
		if (execve(cmd_path2, r.av, environ) == -1)
		{
			printf("minishell: %s: command not found\n", r.av[0]);
			free(cmd_path2);
			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		free(cmd_path2);
	}
	close(fd[0]);
	return (SUCCESS);
}
