#include "../incs/minishell.h"

static int	search_path(char **abs_path, char **exec_path, char **paths, char *cmd)
{
	int			i;
	struct stat	ss;

	i = 0;
	while (paths[i])
	{
		*abs_path = ft_strcjoin(paths[i], cmd, '/');
		if (!*abs_path)
		{
			ft_free_str_arr(paths);
			return (FAILURE);
		}
		if (stat(*abs_path, &ss) == 0 && (ss.st_mode & S_IXUSR))
			break ;
		if (stat(*abs_path, &ss) == 0 && exec_path == NULL)
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

void	print_str(unsigned int i, char *s)
{
	printf("[%d, %s]", i, s);
}
