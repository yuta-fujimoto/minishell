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

void	print_str(unsigned int i, char *s)
{
	printf("[%d, %s]", i, s);
}
