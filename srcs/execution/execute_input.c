#include "../../incs/minishell.h"

static bool	minishell_error(void)
{
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	return (FAILURE);
}

static bool	run_gnu_cmd(t_node node)
{
	pid_t		c_pid;
	char		*cmd_path;
	extern char	**environ;

	cmd_path = create_cmd_path(node);
	if (!cmd_path)
		return (FAILURE);
	c_pid = fork();
	if (c_pid == SYS_ERROR)
		return (free_cmd_path(cmd_path));
	else if (c_pid == 0)
	{
		if (execve(cmd_path, node.av, environ) == SYS_ERROR)
			exit(exec_cmd_error(node.av[0], cmd_path));
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
	redir->old_fd = -1;
	redir->error = false;
}

static bool	has_redirection(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (str_equal(av[i], ">", 2) || str_equal(av[i], "<", 2))
			return (true);
		i++;
	}
	return (false);
}

static char	**create_cmd(char **av, t_redir *redir)
{
	if (has_redirection(av))
		return (set_redirection(av, redir));
	else
		return (av);
}

bool	reset_old_fd(t_redir *redir, int rlt)
{
	int	ret;
	
	ret = 0;
	if (redir->status)
		ret = dup2(redir->safe_fd, redir->old_fd);
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

static bool end_redirection(char **cmd, t_redir *redir, int rlt)
{
	if (cmd)
	{
		free(cmd);
		cmd = NULL;
	}
	if (!reset_old_fd(redir, rlt))
		rlt = FAILURE;
	if (redir->old_fd != STDOUT_FILENO && redir->old_fd != STDIN_FILENO)
	{
		if (!close_fd(redir->old_fd, rlt))
			rlt = FAILURE;
	}
//	if (!close_fd(redir->new_fd, rlt))
//		rlt = FAILURE;
	if (!close_fd(redir->safe_fd, rlt))
		rlt = FAILURE;
	return (rlt);
}
/* dont't need this close due to close on ecex flag???*/

static bool	execute_cmd(t_node node, t_set *set)
{
	int		rlt;
	char	**cmd;
	t_redir	redir;
	
	init_redirection(&redir);
	cmd = create_cmd(node.av, &redir);
	if (!cmd && redir.error)
		return (end_redirection(NULL, &redir, FAILURE));
	if (is_buildin(cmd[0]))
		rlt = run_builtin_cmd(cmd, set);
	else
		rlt = run_gnu_cmd(node);
	if (has_redirection(node.av))
		rlt = end_redirection(cmd, &redir, rlt);
	return (rlt);
}
/*
 * when doing something like '1> filename' or and error like 'ls 999> filename'
 * we might immediately exit failure bcs we are trying to say
 * is_buildin(cmd[0]) on something that is NULL.. also in run_gnu_cmd
 * we are eventually trying to do the same thing... Needs to be made compatible
 * so that at some point, if !cmd because of malloc error, then cmd == NULL,
 * otherwise cmd[0] = ft_strdup(""), perhaps?
 * possibly in create_cmd here, and I'm not sure yet about in pipes
 */

bool	execute_input(t_tree *l, t_set *set)
{
	int		rlt;

	rlt = SUCCESS;
	if (!l)
		return (SUCCESS);
	else if (l->node.flgs == PIPE)
		rlt = execute_pipe(l, set);
	else
	{
		execute_input(l->left, set);
		if (l->node.av)
			rlt = execute_cmd(l->node, set);
		execute_input(l->right, set);
	}
	if (rlt == FAILURE)
		return (minishell_error());
	return (rlt);
}
/*change execute_input*/
