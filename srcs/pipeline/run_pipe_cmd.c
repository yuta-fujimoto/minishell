#include "../../incs/minishell.h"

bool	free_cmd_path(char *cmd_path)
{
	free(cmd_path);
	cmd_path = NULL;
	return (FAILURE);
}

static void	update_pipes(t_pipes *pipes)
{
	if (pipes->status == FIRST_PIPE)
	{
		close(pipes->fd_a[0]);
		dup2(pipes->fd_a[1], STDOUT_FILENO);
		close(pipes->fd_a[1]);
	}
	else if (pipes->status == MIDDLE_PIPE)
	{
		dup2(pipes->fd_a[0], STDIN_FILENO);
		close(pipes->fd_a[0]);
		close(pipes->fd_b[0]);
		dup2(pipes->fd_b[1], STDOUT_FILENO);
	}
	else
	{
		close(pipes->fd_a[1]);
		dup2(pipes->fd_a[0], STDIN_FILENO);
		close(pipes->fd_a[0]);
	}
}

static int	get_cmd_path(t_node *node, char **cmd_path)
{
	if (is_buildin(node->av[0]))
		*cmd_path = NULL;
	else
	{
		*cmd_path = create_cmd_path(node->av);
		if (!*cmd_path)
			return (FAILURE);
	}
	return (SUCCESS);
}

static void	run_child(t_node *node, t_pipes *pipes, t_set *set, t_pipe_info *p_info)
{
	extern char	**environ;
	bool		touch;

	touch = false;
	p_info->cmd = create_cmd(node, p_info->rdr, &touch);
	if (!p_info->cmd && !touch && has_redirection(node))
		exit(EXIT_FAILURE);
	else
		update_pipes(pipes);
	if (touch)
		exit(EXIT_SUCCESS);
	if (is_buildin(p_info->cmd[0]))
	{
		if (run_builtin_cmd(p_info->cmd, set) == FAILURE)
			exit(exec_cmd_error(p_info->cmd[0], p_info->cmd_path));
		exit(EXIT_SUCCESS);
	}
	else if (execve(p_info->cmd_path, p_info->cmd, environ) == -1)
		exit(exec_cmd_error(p_info->cmd[0], p_info->cmd_path));
}

bool	run_pipe_cmd(t_node node, t_pipes *pipes, t_set *set, t_redir *redir)
{
	pid_t		c_pid;
	t_node		*exp_node;
	t_pipe_info	p_info;
	int			rlt;

	p_info.rdr = redir;
	p_info.cmd = NULL;
	exp_node = expansion_node(&node);
	if (exp_node == NULL)
		return (FAILURE);
	if (!exp_node->av)
		return (expansion_node_conclude(exp_node, SUCCESS));
	if (get_cmd_path(exp_node, &p_info.cmd_path) == FAILURE)
		return (FAILURE);// need to return expansion node conclude?
	c_pid = fork();
	if (c_pid < 0)
		return (free_cmd_path(p_info.cmd_path));// need to return expansion node conclude?
	else if (c_pid == 0)
		run_child(exp_node, pipes, set, &p_info);
	else
	{
		if (!wait_options(c_pid))
			return (free_cmd_path(p_info.cmd_path));//need to do end redirection here?// need to return expansion node conclude?
		if (p_info.cmd_path)
			free(p_info.cmd_path);
	}
	if (has_redirection(&node))
		rlt = end_redirection(p_info.cmd, redir, SUCCESS);//should be entering rlt instead of success? I think I might need to use exit status here.. if there is malloc error in child process, I need to be able to put rlt in here.. or I can just separate create_new_cmd from redirection.	
	return (expansion_node_conclude(exp_node, SUCCESS));//should enter rlt isntead of success
}
