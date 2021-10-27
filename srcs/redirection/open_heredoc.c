#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static int	conclude_heredoc(int fds[2])
{
	if (close(fds[1]) == SYS_ERROR)
	{
		close(fds[0]);
		g_sig_info.sys_error = true;
		return (false);
	}
	if (g_sig_info.signal)
	{
		g_sig_info.heredoc_sigint = true;
		if (close(fds[0]) == SYS_ERROR)
		{
			g_sig_info.sys_error = true;
			return (false);
		}
		return (false);
	}
	g_sig_info.heredoc = false;
	return (true);
}

int	handle_heredoc(int fds[2], char *delimiter, bool expand)
{
	char	*line;	

	line = NULL;
	expand = false;//
	g_sig_info.heredoc = true;
	while (1)
	{
		line = readline("> ");
		if (!line || g_sig_info.signal)
		{
			if (!g_sig_info.signal)
			{
				write(STDOUT_FILENO, "\033[A\r> ", 6);
				g_sig_info.heredoc_sigeof = true;
			}
			break ;
		}
		if (str_equal(delimiter, line, ft_strlen(delimiter) + 1))
			break ;// need to add exception for when delimiter is in quotes
		ft_putendl_fd(line, fds[1]);
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
	line = NULL;
	return (conclude_heredoc(fds));
}
