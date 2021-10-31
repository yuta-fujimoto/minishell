#include "../../incs/minishell.h"

static char	**find_paths(bool *malloc_failure)
{
	char	*path;
	char	**paths;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		*malloc_failure = true;
	return (paths);
}

int	create_cmd_path(char **cmd, char **cmd_path, bool *path_error)
{
	char	**paths;
	bool	malloc_failure;

	malloc_failure = false;
	if (ft_strchr(cmd[0], '/') == NULL)
	{	
		paths = find_paths(&malloc_failure);
		if (malloc_failure)
			return (FAILURE);	
		else if (!paths)
		{
			*path_error = true;
			return (SUCCESS);
		}
		if (create_path(cmd[0], paths, cmd_path) == FAILURE)
			return (FAILURE);
	}
	else
	{
		*cmd_path = ft_strdup(cmd[0]);
		if (!*cmd_path)
			return (FAILURE);
	}
	return (SUCCESS);
}
