#include "../../incs/minishell.h"

static bool	minishell_error(void)
{
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

static bool	run_gnu_cmd(char **cmd)
{
	pid_t		c_pid;
	char		*cmd_path;
	extern char	**environ;

	cmd_path = create_cmd_path(cmd);
	if (!cmd_path)
		return (FAILURE);
	c_pid = fork();
	if (c_pid == SYS_ERROR)
		return (free_cmd_path(cmd_path));
	else if (c_pid == 0)
	{
		if (execve(cmd_path, cmd, environ) == SYS_ERROR)
			exit(exec_cmd_error(cmd[0], cmd_path));
	}
	else
	{
		if (!wait_options(c_pid))
			return (free_cmd_path(cmd_path));
		free(cmd_path);
	}
	return (SUCCESS);
}

static void	init_redirection(t_redir *redir)
{
	redir->status = 0;
	redir->safe_fd = -1;
	redir->new_fd = -1;
	redir->stdio_fd = -1;
	redir->redirection = false;
	redir->r_flags = O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC;
	redir->rr_flags = O_RDWR | O_CREAT | O_APPEND | O_CLOEXEC;
	redir->l_flags = O_RDWR | O_CREAT | O_CLOEXEC;
	redir->permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
}

static bool	has_redirection(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (is_rdir(av[i]))
			return (true);
		i++;
	}
	return (false);
}

static char	**create_cmd(char **av, t_redir *redir, bool *touch)
{
	if (has_redirection(av))
		return (ms_redirection(av, redir, touch));
	else
		return (av);
}

bool	reset_stdio_fd(t_redir *redir, int rlt)
{
	int	ret;

	ret = 0;
	if (redir->redirection)
		ret = dup2(redir->safe_fd, redir->stdio_fd);
	if (ret == SYS_ERROR || rlt == FAILURE)
		return (false);
	return (true);
}

bool	close_fd(int fd, int rlt)
{
	int	ret;

	ret = 0;
	if (fd != -1)
		ret = close(fd);
	if (ret == SYS_ERROR || rlt == FAILURE)
		return (false);
	return (true);
}

static bool	end_redirection(char **cmd, t_redir *redir, int rlt)
{
	if (cmd)
	{
		free(cmd);
		cmd = NULL;
	}
	if (!reset_stdio_fd(redir, rlt))
		rlt = FAILURE;
	if (is_open_fd(redir->new_fd))
	{
		if (!close_fd(redir->new_fd, rlt))
			rlt = FAILURE;
	}
	if (!close_fd(redir->safe_fd, rlt))
		rlt = FAILURE;
	return (rlt);
}

static bool	execute_cmd(char **av, t_set *set)
{
	int		rlt;
	char	**cmd;
	t_redir	redir;
	bool	touch;

	touch = false;
	rlt = SUCCESS;
	init_redirection(&redir);
	cmd = create_cmd(av, &redir, &touch);
	if (!cmd && !touch)
		return (end_redirection(NULL, &redir, FAILURE));
	else if (!touch)
	{	
		if (is_buildin(cmd[0]))
			rlt = run_builtin_cmd(cmd, set);
		else
			rlt = run_gnu_cmd(cmd);
	}	
	if (has_redirection(av))
		rlt = end_redirection(cmd, &redir, rlt);
	return (rlt);
}

bool	execute_input(t_tree *l, t_set *set)
{
	if (!l)
		return (SUCCESS);
	else if (l->node.flgs == PIPE)
	{
		if (execute_pipe(l, set) == FAILURE)
			return (minishell_error());
	}
	else
	{
		execute_input(l->left, set);
		if (l->node.av)
		{
			if (execute_cmd(l->node.av, set) == FAILURE)
				return (minishell_error());
		}
		execute_input(l->right, set);
	}
	return (SUCCESS);
}
