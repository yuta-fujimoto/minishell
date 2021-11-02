#include "../../incs/minishell.h"
#include <unistd.h>
#include <fcntl.h>

extern t_sig_info	g_sig_info;

static void	init_redirection(t_redir *redir)
{
	redir->status = 0;
	redir->safe_out = -1;
	redir->new_out = -1;
	redir->safe_in = -1;
	redir->new_in = -1;
	redir->r_flags = O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC;
	redir->rr_flags = O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC;
	redir->l_flags = O_RDONLY | O_CLOEXEC;
	redir->permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
}

bool	set_sys_error(void)
{
	g_sig_info.sys_error = true;
	return (false);
}

bool	redirect_fds(t_redir *redir)
{
	if (redir->new_out != -1)
	{
		redir->safe_out = dup(STDOUT_FILENO);
		if (redir->safe_out == SYS_ERROR)
			return (set_sys_error());
		if (dup2(redir->new_out, STDOUT_FILENO) == SYS_ERROR)
			return (set_sys_error());
	}
	if (redir->new_in != -1)
	{
		redir->safe_in = dup(STDIN_FILENO);
		if (redir->safe_in == SYS_ERROR)
			return (set_sys_error());
		if (dup2(redir->new_in, STDIN_FILENO) == SYS_ERROR)
			return (set_sys_error());
	}
	return (true);
}

bool	ms_redirection(t_node *node, t_redir *redir, t_doclist **hdocs)
{
	int			i;

	i = -1;
	init_redirection(redir);
	while (node->av[++i])
	{
		if (!is_rdir(node->str_flgs[i]))
			continue ;
		if (!set_redirection(node->av, i, redir, *hdocs))
			return (false);
		if (str_equal(node->av[i], "<<", 3))
			(*hdocs) = (*hdocs)->next;
	}
	if (!redirect_fds(redir))
		return (false);
	return (true);
}
