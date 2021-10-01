#include "../incs/minishell.h"

static bool reset_fds(t_redir *redir)
{
	bool	error;

	error = false;
	if (redir->status)
	{
		if (!reset_stdio_fd(redir, SUCCESS))
			error = true;
		redir->status = 0;
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

bool	is_rdir_out(char *cmd_i)
{
	return (cmd_i[0] == '>');
}

bool	is_rdir_in(char *cmd_i)
{
	return (cmd_i[0] == '<');
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
		redir->new_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (redir->status == RRDIR)
		redir->new_fd = open(filename, O_RDWR | O_CREAT | O_APPEND | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (redir->status == LDIR)
		redir->new_fd = open(filename, O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else
		;
	if (redir->new_fd == SYS_ERROR)
		return (false);
	return (true);
}

static bool	redirect_output(char **cmd, int i, t_redir *redir)
{
	int	rlt;

	set_status(cmd[i], redir);
	decide_stdio_fd(redir);
	if (!open_new_fd(cmd[i + 1], redir))
		return (false);
	redir->safe_fd = dup(redir->stdio_fd);
	if (redir->safe_fd == SYS_ERROR)
		return (false);
	rlt = dup2(redir->new_fd, redir->stdio_fd);
	if (rlt == SYS_ERROR)
		return (false);
	return (true);
}

static int	ft_str_arr_len(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
		i++;
	return (i);
}

static int	count_cmd_info(char **cmd)
{
	int	i;
	int	len;

	i = 0;
	len = ft_str_arr_len(cmd);
	while (cmd[i])
	{
		if (is_rdir_out(cmd[i]) || is_rdir_in(cmd[i]))	
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
		if (is_rdir_out(cmd[i]) || is_rdir_in(cmd[i]))
			i += 2;/*this could be trouble, but it should already be syntax error boefre this piont*/
		else
			new_cmd[j++] = cmd[i++];
	}
	new_cmd[j] = NULL;
	return (new_cmd);
}

char **set_redirection(char **cmd, t_redir *redir, bool *touch)
{
	int		i;
	int		rlt;

	i = 0;
	while (cmd[i])
	{
		if (is_rdir_out(cmd[i]) || is_rdir_in(cmd[i]))
		{
			if (!reset_fds(redir))
				return (NULL);
		}
		if (is_rdir_out(cmd[i]))
			rlt = redirect_output(cmd, i, redir);
		else if (is_rdir_in(cmd[i]))
			rlt = redirect_output(cmd, i, redir);	
		if (!rlt)
			return (NULL);
		i++;
	}
	return (create_new_cmd(cmd, touch));
}
