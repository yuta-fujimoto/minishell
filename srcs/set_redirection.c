#include "../incs/minishell.h"

static bool	reset_fds(t_redir *redir)
{
	bool	error;

	error = false;
	if (redir->status)
	{
		if (!reset_stdio_fd(redir, SUCCESS))
			error = true;
		redir->status = 0;
		redir->redirection = false;
	}
	redir->stdio_fd = -1;
	if (!close_fd(redir->new_fd, SUCCESS))
		error = true;
	redir->new_fd = -1;
	if (!close_fd(redir->safe_fd, SUCCESS))
		error = true;
	redir->safe_fd = -1;
	if (error)
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

static void	decide_stdio_fd(t_redir *redir)
{
	if (redir->status == RDIR || redir->status == RRDIR)
		redir->stdio_fd = STDOUT_FILENO;
	else
		redir->stdio_fd = STDIN_FILENO;
}

static bool	open_new_fd(char *filename, t_redir *redir)
{
	if (redir->status == RDIR)
		redir->new_fd = open(filename, redir->r_flags, redir->permissions);
	else if (redir->status == RRDIR)
		redir->new_fd = open(filename, redir->rr_flags, redir->permissions);
	else if (redir->status == LDIR)
		redir->new_fd = open(filename, redir->l_flags, redir->permissions);
	else
		;
	if (redir->new_fd == SYS_ERROR)
		return (false);
	return (true);
}

static bool	set_redirection(char **cmd, int i, t_redir *redir)
{
	set_status(cmd[i], redir);
	decide_stdio_fd(redir);
	if (!open_new_fd(cmd[i + 1], redir))
		return (false);
	redir->safe_fd = dup(redir->stdio_fd);
	if (redir->safe_fd == SYS_ERROR)
		return (false);
	if (dup2(redir->new_fd, redir->stdio_fd) == SYS_ERROR)
		return (false);
	redir->redirection = true;
	return (true);
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
		if (!reset_fds(redir))
			return (NULL);
		if (!set_redirection(cmd, i, redir))
			return (NULL);
		i++;
	}
	return (create_new_cmd(cmd, touch));
}
