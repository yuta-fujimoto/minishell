#include "../../incs/minishell.h"

static char	*create_path_for_cd(char **cmps, int end)
{
	char	*path;
	char	*tmp;
	int		i;

	i = 0;
	if (end == 0)
		return (ft_strdup("/"));
	path = ft_strdup("");
	if (!path)
		return (NULL);
	while (cmps[i] && i < end)
	{
		tmp = path;
		path = ft_strcjoin(path, cmps[i], '/');
		free(tmp);
		if (!path)
			return (NULL);
		i++;
	}
	return (path);
}

static bool	is_valid_path(char **cmps, int end)
{
	char		*path;
	struct stat	st;

	path = create_path_for_cd(cmps, end);
	if (!path)
		return (false);
	if (stat(path, &st) == -1)
	{
		errno = ENOENT;
		free(path);
		return (false);
	}
	free(path);
	return (true);
}

static void	skip_dot(char **cmps)
{
	int	i;
	int	j;
	int	cmps_len;

	i = 0;
	j = 0;
	cmps_len = ft_str_arr_len(cmps);
	while (i < cmps_len)
	{
		if (!str_equal(cmps[i], ".", 2))
			cmps[j++] = cmps[i];
		else
			free(cmps[i]);
		i++;
	}
	while (j <= i)
		cmps[j++] = NULL;
}

static bool	skip_dot_dot(char **cmps)
{
	int	i;
	int	j;

	j = 0;
	i = ft_str_arr_search(cmps, "..");
	if (i != 0 && !is_valid_path(cmps, i))
		return (false);
	free(cmps[i]);
	if (i != 0)
	{
		j = 1;
		free(cmps[i - 1]);
		cmps[i - 1] = cmps[i + 1];
	}
	cmps[i] = cmps[i + j + 1];
	while (cmps[i])
	{
		if (cmps[i + 1 + j])
			cmps[i] = cmps[i + 1 + j];
		else
			cmps[i] = NULL;
		i++;
	}
	return (true);
}

char	*canonical_path(char *pathname)
{
	char	**cmps;
	char	*cano_path;

	cmps = ft_split(pathname, '/');
	if (!cmps)
		return (NULL);
	skip_dot(cmps);
	while (ft_str_arr_search(cmps, "..") != -1)
	{
		if (!skip_dot_dot(cmps))
		{
			free(pathname);
			ft_free_str_arr(cmps);
			return (NULL);
		}
	}
	cano_path = create_path_for_cd(cmps, ft_str_arr_len(cmps));
	free(pathname);
	ft_free_str_arr(cmps);
	return (cano_path);
}
