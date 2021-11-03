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

static int	create_cmd_path_from_path(char **cmd,
	char **cmd_path, bool *path_error)
{
	char	**paths;
	bool	malloc_failure;

	malloc_failure = false;
	paths = find_paths(&malloc_failure);
	if (malloc_failure)
		return (FAILURE);
	else if (!paths || !paths[0])
	{
		ft_free_str_arr(paths);
		*path_error = true;
		*cmd_path = ft_strjoin("./", cmd[0]);
		if (!*cmd_path)
			return (FAILURE);
		return (SUCCESS);
	}
	if (create_path(cmd[0], paths, cmd_path) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	create_cmd_path(char **cmd, char **cmd_path, bool *path_error)
{
	if (ft_strchr(cmd[0], '/') == NULL)
		return (create_cmd_path_from_path(cmd, cmd_path, path_error));
	else
	{
		*cmd_path = ft_strdup(cmd[0]);
		if (!*cmd_path)
			return (FAILURE);
	}
	return (SUCCESS);
}
