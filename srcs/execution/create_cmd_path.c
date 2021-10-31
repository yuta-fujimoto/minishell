#include "../../incs/minishell.h"

static char	**find_paths(void)
{
	char	*path;
	char	**paths;

	path = getenv("PATH");
	paths = NULL;
	paths = ft_split(path, ':');
	return (paths);
}

int	create_cmd_path(char **cmd, char **cmd_path)
{
	char	**paths;

	if (ft_strchr(cmd[0], '/') == NULL)
	{
		paths = find_paths();
		if (!paths)
			return (FAILURE);
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
