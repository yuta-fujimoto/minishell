#include "../incs/minishell.h"

char	*create_path(char *cmd, char **paths)
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
			return (NULL);
		}
		if (stat(abs_path, &ss) == 0)
			break ;
		free(abs_path);
		abs_path = NULL;
		i++;
	}
	ft_free_str_arr(paths);
	if (!abs_path)
		return (ft_strdup(cmd));
	return (abs_path);
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
	printf("[%d, %s]",i ,s);
}
