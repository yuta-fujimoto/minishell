#include "../incs/minishell.h"
#include <unistd.h>
#include <sys/stat.h>

extern t_sig_info	g_sig_info;

static int	search_path(char **abs_path, char **exec_path,
		char **paths, char *cmd)
{
	int			i;
	struct stat	ss;

	i = 0;
	while (paths[i])
	{
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			*abs_path = ft_strcjoin(paths[i], cmd, '/');
		else
			*abs_path = ft_strjoin(paths[i], cmd);
		if (!*abs_path)
		{
			ft_free_str_arr(paths);
			return (FAILURE);
		}
		if (stat(*abs_path, &ss) == 0 && (ss.st_mode & S_IXUSR))
			break ;
		if (stat(*abs_path, &ss) == 0 && *exec_path == NULL)
			*exec_path = *abs_path;
		else
			free(*abs_path);
		*abs_path = NULL;
		i++;
	}
	return (SUCCESS);
}

int	create_path(char *cmd, char **paths, char **cmd_path)
{
	char	*abs_path;
	char	*exec_path;

	abs_path = NULL;
	exec_path = NULL;
	if (search_path(&abs_path, &exec_path, paths, cmd) == FAILURE)
		return (FAILURE);
	ft_free_str_arr(paths);
	if (abs_path)
	{
		*cmd_path = abs_path;
		if (exec_path)
			free(exec_path);
	}
	else
		*cmd_path = exec_path;
	return (SUCCESS);
}

bool	str_equal(char *s1, char *s2, size_t n)
{
	if (!s1 || !s2)
		return (false);
	if (ft_strncmp(s1, s2, n) == 0)
		return (true);
	return (false);
}

bool	mod_termios_attr(t_set *set, int init)
{
	int	lflag;
	int	vquit;

	if (init)
	{
		lflag = C_LFLAGS;
		vquit = false;
	}
	else
	{
		lflag = set->safe_c_lflag;
		vquit = set->safe_c_vquit;
	}
	set->t.c_lflag = lflag;
	set->t.c_cc[VQUIT] = vquit;
	if (isatty(STDIN_FILENO)
		&& tcsetattr(STDIN_FILENO, TCSANOW, &set->t) == SYS_ERROR)
	{
		perror(NULL);
		g_sig_info.sys_error = true;
		return (false);
	}
	return (true);
}

void	free_set(t_set *set)
{
	ft_lstclear(&set->lst, free);
	free_tree(set->tree);
	set->tree = NULL;
	if (set->input)
	{
		free(set->input);
		set->input = NULL;
	}
	if (set->heredoc_lst)
	{
		close_heredocs(set->heredoc_lst);
		ft_doclstclear(&set->heredoc_lst);
	}
	if (g_sig_info.sys_error)
		ms_exit(set, EXIT_FAILURE, true);
}
