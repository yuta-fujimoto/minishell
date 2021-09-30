#include "../incs/minishell.h"

/*static char	*redirection_error(t_redir *redir)
{
	rlt = end_redirection(NULL, redir, SUCCESS);
	if (rlt == FAILURE)
		redir->error = true;
	return (NULL);
}*/

static bool reset_fds(t_redir *redir)
{
	if (redir->status)
	{
		if (!reset_old_fd(redir, SUCCESS))
			redir->error = true;
		redir->status = 0;
	}
	if (redir->old_fd != STDOUT_FILENO && redir->old_fd != STDIN_FILENO)
	{
		if (!close_fd(redir->old_fd, SUCCESS))
			redir->error = true;
		redir->old_fd = -1;
	}
	if (!close_fd(redir->new_fd, SUCCESS))
		redir->error = true;
	redir->new_fd = -1;
	if (!close_fd(redir->safe_fd, SUCCESS))
		redir->error = true;
	redir->safe_fd = -1;
	if (redir->error)
		return (false);
	return (true);
}

static bool	is_rdir_out(char *cmd_i)
{
	return (cmd_i[0] == '>');
}

static bool	is_rdir_in(char *cmd_i)
{
	return (cmd_i[0] == '<');
}

static bool	is_open_fd(int fd)
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

static bool	decide_old_fd(char **cmd, int i, t_redir *redir, int stdio)
{
	redir->old_fd = stdio;
	if (i == 0)
		return (true);
	else if (ft_isdigit(cmd[i - 1][0]))
	{
		redir->old_fd = ft_atoi(cmd[i - 1]);
		if (redir->old_fd < 0 || redir->old_fd > 9 || is_open_fd(redir->old_fd))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd[i - 1], STDERR_FILENO);
			ft_putendl_fd(": Bad file descriptor", STDERR_FILENO);
			return (false);
		}
	}
	return (true);
}
/*
 * maybe is_open_fd is unecessary, for example in bash we can use 0, 1, and 2....
 * look into this further..
 * perhaps the problem I had before was also that I was tyring to open an already
 * opened file! so maybe instead of useing is_open_fd in decide_old_fd, I can
 * use it in open_old_fd, so that I can just return if is_open_fd
 * Will do that now, but leave the notes in case i need to chagne it back.
 */

static bool	open_old_fd(char *filename, t_redir *redir)
{
	if (is_open_fd(redir->old_fd))
		return (true);
	if (redir->status == RDIR)
		redir->new_fd = open(filename, O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (redir->status == RRDIR)
		redir->new_fd = open(filename, O_RDWR | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (redir->status == LDIR)
		;
	else
		;
	if (redir->new_fd == SYS_ERROR)
	{
		redir->error = true;
		return (false);
	}
	return (true);
}

static bool	redirect_output(char **cmd, int i, t_redir *redir)
{
	int	rlt;

	set_status(cmd[i], redir);
	if (!decide_old_fd(cmd, i, redir, STDOUT_FILENO))
		return (false);
	if (!open_old_fd(cmd[i + 1], redir))
		return (false);
	redir->safe_fd = dup(redir->old_fd);
	if (redir->safe_fd == SYS_ERROR)
	{
		redir->error = true;
		return (false);
	}
	rlt = dup2(redir->new_fd, redir->old_fd);
	if (redir->safe_fd == SYS_ERROR)
	{
		redir->error = true;
		return (false);
	}
	return (true);
}
/*
 * in the end, I think I can make redirect_output(), just ms_redirct() and it can handle everything
 * just create new funcs for dup(), and dupe2() that are similar to set_status, decide_old_fd, and
 * open_old_fd.. 
 */

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
		{
			len -= 2;
			if (i != 0 && ft_isdigit(cmd[i - 1][0]))
				len--;
		}
		i++;
	}
	return (len);
}

static char	**create_new_cmd(char **cmd)
{
	int		i;
	int		j;
	int		new_cmd_len;
	char	**new_cmd;

	i = 0;
	j = 0;
	new_cmd_len = count_cmd_info(cmd);
	new_cmd = malloc(sizeof(char **) * new_cmd_len + 1);
	if (!new_cmd)
		return (NULL);
	while (cmd[i])
	{
		if (ft_isdigit(cmd[i][0]) && (is_rdir_out(cmd[i + 1]) || is_rdir_in(cmd[i + 1])))
			i++;
		else if (is_rdir_out(cmd[i + 1]) || is_rdir_in(cmd[i + 1]))
			i += 2;/*this could be trouble, but it should already be syntax error boefre this piont*/
		else
			new_cmd[j++] = cmd[i++];
	}
	new_cmd[j] = NULL;
	return (new_cmd);
}

char **set_redirection(char **cmd, t_redir *redir)
{
	int		i;
	int		rlt;
	char	**new_cmd;

	i = 0;
	while (cmd[i])
	{
		if (!reset_fds(redir))
			return (NULL);
		if (is_rdir_out(cmd[i]))
			rlt = redirect_output(cmd, i, redir);
		else if (is_rdir_in(cmd[i]))
			;//ret = redirect_input(cmd, i, redir);
		if (!rlt)
			return (NULL);
		i++;
	}
	new_cmd = create_new_cmd(cmd);
	if (!new_cmd)
		redir->error = true;
	return (new_cmd);
}
