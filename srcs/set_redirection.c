#include "../incs/minishell.h"

/*static bool	check_for_heredock(t_redir *redir)
{
	if (redir->status != LLDIR)
		return (true);
	if (redir->heredoc_bkp != -1)
	{
		if (close(redir->heredoc_bkp) == SYS_ERROR)
			return (false);
	}
	redir->heredoc_bkp = redir->new_fd;
	redir->new_fd = -1;
}*/

static bool	reset_fds(t_redir *redir)
{
	int	rlt;

	rlt = 0;
	if (redir->status == RDIR || redir->status == RRDIR)
        { 
//                rlt = dup2(redir->safe_out, STDOUT_FILENO);
//                redir->safe_out = -1; 
//                redir->out = false;
		if (!close_fd(redir->new_out, SUCCESS))
			rlt = -1 ;
		redir->new_out = -1;
        }   
        else if ((redir->status == LDIR || redir->status == LLDIR))
        {   
//                rlt = dup2(redir->safe_in, STDIN_FILENO);    
//                redir->safe_in = -1; 
//                redir->in = false;
		if (!close_fd(redir->new_in, SUCCESS))
			rlt = true;
		redir->new_in = -1;
        }   
	if (rlt == SYS_ERROR)
		return (false);
	return (true);
}

bool	is_rdir(char *cmd_i)
{
	return (cmd_i[0] == '>' || cmd_i[0] == '<');
}

bool	is_open_fd(int fd)
{
	struct stat	buf;

	return (fstat(fd, &buf) == 0);
}

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

/*static void	decide_stdio_fd(t_redir *redir)
{
	if (redir->status == RDIR || redir->status == RRDIR)
		redir->stdio_fd = STDOUT_FILENO;
	else
		redir->stdio_fd = STDIN_FILENO;
}*/

static int	open_heredoc(char *delimiter)
{
	char	*line;
	int		new_in;
	int		fds[2];

	if (pipe(fds) == SYS_ERROR)
		return (SYS_ERROR);
	new_in = fds[0];
	while (1)
	{
		line = readline("> ");//if !line what do??
		if (str_equal(delimiter, line, ft_strlen(delimiter) + 1))
			break ;// need to add exception for when delimiter is in quotes
		ft_putendl_fd(line, fds[1]);
		free(line);
		line = NULL;
	}
	free(line);
	line = NULL;
	if (close(fds[1]) == SYS_ERROR)
		return (SYS_ERROR);
	return (new_in);
}

static bool	open_new_fd(char *filename, t_redir *redir)
{
	if (redir->status == RDIR)
		redir->new_out = open(filename, redir->r_flags, redir->permissions);
	else if (redir->status == RRDIR)
		redir->new_out = open(filename, redir->rr_flags, redir->permissions);
	else if (redir->status == LDIR)
		redir->new_in = open(filename, redir->l_flags, redir->permissions);
	else
		redir->new_in = open_heredoc(filename);
	if ((redir->status == RDIR || redir->status == RRDIR) && redir->new_out == SYS_ERROR)
			return (false);
	if ((redir->status == LDIR || redir->status == LLDIR) && redir->new_in == SYS_ERROR)
			return (false);
	return (true);
}

static bool	redirect_fds(t_redir *redir)
{
	if (redir->new_out != -1)
	{
		redir->safe_out = dup(STDOUT_FILENO);
		if (redir->safe_out == SYS_ERROR)
			return (false);
		if (dup2(redir->new_out, STDOUT_FILENO) == SYS_ERROR)
			return (false);
	}
	if (redir->new_in != -1)
	{
		redir->safe_in = dup(STDIN_FILENO);
		if (redir->safe_in == SYS_ERROR)
			return (false);
		if (dup2(redir->new_in, STDIN_FILENO) == SYS_ERROR)
			return (false);
	}
	return (true);
}

static bool	set_redirection(char **cmd, int i, t_redir *redir)
{
	set_status(cmd[i], redir);
	if (!reset_fds(redir))
		return (false);
	if (!open_new_fd(cmd[i + 1], redir))
		return (false);
	return (true);
/*	if (redir->status == RDIR || redir->status == RRDIR)
		return (redirect_fds(redir, &redir->safe_out, &redir->new_out, STDOUT_FILENO));
	else
		return (redirect_fds(redir, &redir->safe_in, &redir->new_in, STDIN_FILENO));*/
}

static int	count_cmd_info(char **cmd)
{
	int	i;
	int	len;

	i = 0;
	len = ft_str_arr_len(cmd);
	while (cmd[i])
	{
		if (is_rdir(cmd[i]))
			len -= 2;
		i++;
	}
	return (len);
}

static char	**create_new_cmd(char **cmd, bool *touch)
{
	int		i;
	int		j;
	int		new_cmd_len;
	char	**new_cmd;

	i = 0;
	j = 0;
	new_cmd_len = count_cmd_info(cmd);
	if (!new_cmd_len)
	{
		*touch = true;
		return (NULL);
	}
	new_cmd = malloc(sizeof(char **) * new_cmd_len + 1);
	if (!new_cmd)
		return (NULL);
	while (cmd[i])
	{
		if (is_rdir(cmd[i]))
			i += 2;/*this could be trouble, ensure if > is not followed by anything, it is syntax error*/
		else
			new_cmd[j++] = cmd[i++];
	}
	new_cmd[j] = NULL;
	i = 0;
	return (new_cmd);
}

char	**ms_redirection(char **cmd, t_redir *redir, bool *touch)
{
	int		i;

	i = -1;
	while (cmd[++i])
	{
		if (!is_rdir(cmd[i]))
			continue ;
		if (!set_redirection(cmd, i, redir))
			return (NULL);
	}	
	if (!redirect_fds(redir))
		return (NULL);
	return (create_new_cmd(cmd, touch));
}
