#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static bool	end_pipe(t_node *n, t_pipe_info *p_info, int rlt)
{
	if (p_info->cmd_path)
		free(p_info->cmd_path);
	if (p_info->cmd && has_redirection(n))
		free(p_info->cmd);
	return (expansion_node_conclude(n, rlt));
}

static int	pipe_cmd_not_found(t_node *n, t_pipe_info *p_info, t_pipes *pipes)
{
	if (p_info->path_error)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(n->av[0], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(n->av[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	g_sig_info.exit_status = 127;
	if (!ft_pidlstadd_back(&pipes->pidlst, ft_pidlstnew(0)))
		return (end_pipe(n, p_info, FAILURE));
	return (end_pipe(n, p_info, SUCCESS));
}

static int	get_cmd_path(char **new_cmd, char **cmd_path, bool *path_error)
{
	if (is_buildin(new_cmd[0]))
		*cmd_path = NULL;
	else
	{
		if (create_cmd_path(new_cmd, cmd_path, path_error) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

static bool	init_pipe_cmd(t_node *exp_node, t_pipe_info *p_info, t_redir *redir)
{
	p_info->rdr = redir;
	p_info->touch = false;
	p_info->cmd = NULL;
	p_info->cmd_path = NULL;
	p_info->path_error = false;
	if (has_redirection(exp_node))
		p_info->cmd = create_new_cmd(exp_node, &p_info->touch);
	else
		p_info->cmd = exp_node->av;
	if (!p_info->cmd && !p_info->touch && has_redirection(exp_node))
		return (FAILURE);
	if (p_info->touch)
		return (SUCCESS);
	if (get_cmd_path(p_info->cmd, &p_info->cmd_path, &p_info->path_error) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

bool	has_heredoc(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (str_equal(av[i], "<<", 3))
			return (true);
		i++;
	}
	return (false);
}

static void	update_heredocs(t_node exp_node, t_set *set)
{
	int	i;

	i = -1;
	if (has_heredoc(exp_node.av))
	{
		while (exp_node.av[++i])
		{
			if (str_equal(exp_node.av[i], "<<", 3))
				set->tmp_hdocs = set->tmp_hdocs->next;
		}
	}
}

bool	run_pipe_cmd(t_node node, t_pipes *pipes, t_set *set, t_redir *redir)
{
	pid_t		c_pid;
	t_node		*exp_node;
	t_pipe_info	p_info;

	exp_node = expansion_node(&node);
	if (exp_node == NULL)
		return (FAILURE);
	if (!exp_node->av)
		return (expansion_node_conclude(exp_node, SUCCESS));
	if (init_pipe_cmd(exp_node, &p_info, redir) == FAILURE)
		return (end_pipe(exp_node, &p_info, FAILURE));
	if (!p_info.cmd_path && !p_info.touch && !is_buildin(p_info.cmd[0]))
		return (pipe_cmd_not_found(exp_node, &p_info, pipes));
	c_pid = fork();
	if (c_pid < 0)
		return (end_pipe(exp_node, &p_info, FAILURE));
	else if (c_pid == 0)
		run_child(exp_node, pipes, set, &p_info);
	else
	{
		update_heredocs(*exp_node, set);
		if (!ft_pidlstadd_back(&pipes->pidlst, ft_pidlstnew(c_pid)))
			return (end_pipe(exp_node, &p_info, FAILURE));
	}
	return (end_pipe(exp_node, &p_info, SUCCESS));
}
