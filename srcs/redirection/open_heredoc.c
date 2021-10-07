#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static int	prepare_heredoc(int fds[2], int *new_in, char **line)
{

	if (pipe(fds) == SYS_ERROR)
		return (false);
	*new_in = fds[0];
	*line = NULL;
	if (g_sig_info.signal == SIGINT)
		g_sig_info.signal = 0;
	g_sig_info.heredoc = true;
	return (true);
}

static int	conclude_heredoc(int fds[2], int new_in)
{
	if (close(fds[1]) == SYS_ERROR)
		return (SYS_ERROR);
	if (g_sig_info.signal)
	{	
		if (close(fds[0]) == SYS_ERROR)
			return (SYS_ERROR);
		return (SIGINT_CALL);
	}
	g_sig_info.heredoc = false;
	return (new_in);
}

int	open_heredoc(char *delimiter)
{
	char	*line;
	int		new_in;
	int		fds[2];

	if (!prepare_heredoc(fds, &new_in, &line))
		return (SYS_ERROR);
	while (1)
	{
		line = readline("> ");
		if (!line || g_sig_info.signal)
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
	return (conclude_heredoc(fds, new_in));
}
