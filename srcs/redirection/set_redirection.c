#include "../../incs/minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

extern t_sig_info	g_sig_info;

static void	set_status(char *cmd_i, t_redir *redir)
{
	if (str_equal(cmd_i, ">>", 3))
		redir->status = RRDIR;
	else if (str_equal(cmd_i, "<<", 3))
		redir->status = LLDIR;
	else if (str_equal(cmd_i, ">", 2))
		redir->status = RDIR;
	else
		redir->status = LDIR;
}

static bool	reset_fds(t_redir *redir)
{
	if (redir->status == RDIR || redir->status == RRDIR)
	{
		close_fd(redir->new_out, SUCCESS);
		redir->new_out = -1;
	}
	else if (redir->status == LDIR || redir->status == LLDIR)
	{
		close_fd(redir->new_in, SUCCESS);
		redir->new_in = -1;
	}
	if (g_sig_info.sys_error)
		return (false);
	return (true);
}

bool	is_acceptable_error(int errnum)
{
	return (errnum == ENOENT || errnum == EACCES || errnum == ENOTDIR
		|| errnum == EISDIR);
}

static bool	check_new_fd(char *filename, t_redir *redir)
{
	if ((redir->status == RDIR || redir->status == RRDIR)
		&& redir->new_out == SYS_ERROR)
		redir->nofile = true;
	if ((redir->status == LDIR)
		&& redir->new_in == SYS_ERROR)
		redir->nofile = true;
	if (!redir->nofile)
		return (true);
	if (is_acceptable_error(errno))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		g_sig_info.exit_status = EXIT_FAILURE;
		perror(NULL);
	}
	else
		g_sig_info.sys_error = true;
	return (false);
}

bool	set_redirection(char **cmd, int i, t_redir *redir, t_doclist *hdocs)
{
	set_status(cmd[i], redir);
	if (!reset_fds(redir))
		return (false);
	if (redir->status == RDIR)
		redir->new_out = open(cmd[i + 1], redir->r_flags, redir->permissions);
	else if (redir->status == RRDIR)
		redir->new_out = open(cmd[i + 1], redir->rr_flags, redir->permissions);
	else if (redir->status == LDIR)
		redir->new_in = open(cmd[i + 1], redir->l_flags);
	else
		redir->new_in = hdocs->fds[0];
	return (check_new_fd(cmd[i + 1], redir));
}
