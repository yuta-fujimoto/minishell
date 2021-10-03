#include "../../incs/minishell.h"

static void	init_redirection(t_redir *redir)
{
	redir->status = 0;
	redir->in = false;
	redir->out = false;
	redir->safe_out = -1;
	redir->new_out = -1;
	redir->safe_in = -1;
	redir->new_in = -1;
	redir->r_flags = O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC;
	redir->rr_flags = O_RDWR | O_CREAT | O_APPEND | O_CLOEXEC;
	redir->l_flags = O_RDWR | O_CREAT | O_CLOEXEC;
	redir->permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
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
	init_redirection(redir);
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
