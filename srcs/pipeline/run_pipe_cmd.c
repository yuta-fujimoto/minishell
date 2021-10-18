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

	if (has_redirection(node))
	{
		if (!ms_redirection(node, p_info->rdr))
			exit(EXIT_FAILURE);
	}
	else
		update_pipes(pipes);
	if (p_info->touch)
		exit(EXIT_SUCCESS);
	if (is_buildin(p_info->cmd[0]))
	{
		if (run_builtin_cmd(p_info->cmd, set) == FAILURE)
			exit(exec_cmd_error(p_info->cmd[0], p_info->cmd_path));
		exit(EXIT_SUCCESS);
	}
	else if (execve(p_info->cmd_path, p_info->cmd, environ) == -1)
		exit(exec_cmd_error(p_info->cmd[0], p_info->cmd_path));
}//need to end redirection in here somehow?? in update pipes, I close all pipes after changing direction.. maybe the same should be done (in if has redirection.. just redirect and then close);

static bool	init_pipe_cmd(t_node *exp_node, t_pipe_info *p_info, t_redir *redir)
{
	p_info->rdr = redir;
	p_info->touch = false;
	p_info->cmd = NULL;
	p_info->cmd_path = NULL;
	if (get_cmd_path(exp_node, &p_info->cmd_path) == FAILURE)
		return (FAILURE);
	p_info->cmd = get_cmd(exp_node, redir, &p_info->touch);
	if (!p_info->cmd && !p_info->touch && has_redirection(exp_node))
		return (FAILURE);
	return (SUCCESS);
}

static bool conclude_pipe_section(t_node *exp_node, t_pipe_info *p_info, int rlt)
{
	if (p_info->cmd_path)
		free(p_info->cmd_path);
	if (p_info->cmd && has_redirection(exp_node))
		free(p_info->cmd);
	return (expansion_node_conclude(exp_node, rlt));
}

bool	run_pipe_cmd(t_node node, t_pipes *pipes, t_set *set, t_redir *redir)
{
	pid_t		c_pid;
	t_node		*exp_node;
	t_pipe_info	p_info;
	int			rlt;

	rlt = SUCCESS;
	exp_node = expansion_node(&node);
	if (exp_node == NULL)
		return (FAILURE);
	if (!exp_node->av)
		return (expansion_node_conclude(exp_node, SUCCESS));
	if (init_pipe_cmd(exp_node, &p_info, redir) == FAILURE)
		return (conclude_pipe_section(exp_node, &p_info, FAILURE));
	c_pid = fork();
	if (c_pid < 0)
		return (conclude_pipe_section(exp_node, &p_info, FAILURE));
	else if (c_pid == 0)
		run_child(exp_node, pipes, set, &p_info);
	else
	{
		if (!wait_options(c_pid))
			return (conclude_pipe_section(exp_node, &p_info, FAILURE));
	}
	if (has_redirection(&node))//do I even need to end redirection?? here?? redirection only happens in child procss
		rlt = end_redirection(NULL, redir, SUCCESS);
	return (conclude_pipe_section(exp_node, &p_info, rlt));
}
