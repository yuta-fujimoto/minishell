#include "../incs/minishell.h"

int	create_path(char *cmd, char **paths, char **cmd_path)
{
	char		*abs_path;
	int			i;
	struct stat	ss;

	abs_path = NULL;
	i = 0;
	while (paths[i])
	{
		abs_path = ft_strcjoin(paths[i], cmd, '/');
		if (!abs_path)
		{
			ft_free_str_arr(paths);
			return (FAILURE);
		}
		if (stat(abs_path, &ss) == 0 && (ss.st_mode & S_IXUSR))
			break ;
		free(abs_path);
		abs_path = NULL;
		i++;
	}
	ft_free_str_arr(paths);
	*cmd_path = abs_path;
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

void	mod_termios_attr(t_set *set, int init)
{
	unsigned int	lflag;
	unsigned char	vquit;

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
	if (isatty(STDIN_FILENO) && tcsetattr(STDIN_FILENO, TCSANOW, &set->t) == SYS_ERROR)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}
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
}

void	ms_exit(t_set *set, int exit_status)
{
	extern char	**environ;

	free_environ();
	environ = set->safe_envrion;
	mod_termios_attr(set, false);
	exit(exit_status);
}
