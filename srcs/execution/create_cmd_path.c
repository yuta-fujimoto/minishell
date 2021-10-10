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

char	*create_cmd_path(char **cmd)
{
	char	**paths;
	char	*cmd_path;

	if (cmd[0][0] != '/')
	{
		paths = find_paths();
		if (!paths)
			return (NULL);
		cmd_path = create_path(cmd[0], paths);
	}
	else
		cmd_path = ft_strdup(cmd[0]);
	return (cmd_path);
}
