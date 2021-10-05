#include "../../incs/minishell.h"

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
	int	rlt;

	rlt = 0;
	if (redir->status == RDIR || redir->status == RRDIR)
	{
		if (!close_fd(redir->new_out, SUCCESS))
			rlt = -1 ;
		redir->new_out = -1;
	}
	else if ((redir->status == LDIR || redir->status == LLDIR))
	{
		if (!close_fd(redir->new_in, SUCCESS))
			rlt = -1;
		redir->new_in = -1;
	}
	if (rlt == SYS_ERROR)
		return (false);
	return (true);
}

static int	open_heredoc(char *delimiter)
{
	char	*line;
	int		new_in;
	int		fds[2];

	if (pipe(fds) == SYS_ERROR)
		return (SYS_ERROR);
	new_in = fds[0];
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;	
		if (str_equal(delimiter, line, ft_strlen(delimiter) + 1))
			break ;// need to add exception for when delimiter is in quotes
		ft_putendl_fd(line, fds[1]);
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
	line = NULL;
	if (close(fds[1]) == SYS_ERROR)
		return (SYS_ERROR);
	return (new_in);
}

static bool	check_new_fd(char *filename, t_redir *redir)
{

	if ((redir->status == RDIR || redir->status == RRDIR)
		&& redir->new_out == SYS_ERROR)
		redir->perror = true;
	if ((redir->status == LDIR || redir->status == LLDIR)
		&& redir->new_in == SYS_ERROR)
		redir->perror = true;
	if (!redir->perror)
		return (true);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
	return (false);
}

bool	set_redirection(char **cmd, int i, t_redir *redir)
{
	set_status(cmd[i], redir);
	if (!reset_fds(redir))
		return (false);
	if (redir->status == RDIR)
		redir->new_out = open(cmd[i + 1], redir->r_flags, redir->permissions);
	else if (redir->status == RRDIR)
		redir->new_out = open(cmd[i + 1], redir->rr_flags, redir->permissions);
	else if (redir->status == LDIR)
		redir->new_in = open(cmd[i + 1], redir->l_flags, redir->permissions);
	else
		redir->new_in = open_heredoc(cmd[i + 1]);
	return (check_new_fd(cmd[i + 1], redir));
}
