#include "../../incs/minishell.h"

static char	*free_canonical(char **cmps, char *pathname, int error)
{
	if (error != 0)
		errno = error;
	free(pathname);
	ft_free_str_arr(cmps);
	return (NULL);
}

static int	update_path(char **pathname, char *newcmp)
{
	char	*tmp;

	if (!*pathname)
	{
		*pathname = ft_strjoin("/", newcmp);
		if (!*pathname)
			return (FAILURE);
		else
			return (SUCCESS);
	}
	if (!newcmp)
		return (SUCCESS);
	tmp = *pathname;
	if ((*pathname)[ft_strlen(*pathname) - 1] == '/')
		*pathname = ft_strjoin(*pathname, newcmp);
	else
		*pathname = ft_strcjoin(*pathname, newcmp, '/');
	if (!*pathname)
		return (FAILURE);
	free(tmp);
	return (SUCCESS);
}

static bool	is_valid_path(char **cmps, int end)
{
	char		*pathname;
	struct stat	st;
	int			i;

	i = 0;
	pathname = NULL;
	while (i <= end)
	{
		if (update_path(&pathname, cmps[i]) == FAILURE)
			return (false);
		i++;
	}
	if (stat(pathname, &st) == -1)
	{
		free(pathname);
		return (false);
	}
	free(pathname);
	return (true);
}

static int	skip_dot(char **cmps, char *pathname, int *i, char *free_if_error)
{
	if (pathname[0] == '/')
	{
		while (cmps[*i] && str_equal(cmps[*i], "..", 3))
			(*i)++;
	}
	while (cmps[*i] && cmps[*i + 1] && str_equal(cmps[*i + 1], "..", 3)
		&& !str_equal(cmps[*i], "..", 3))
	{
		if (!is_valid_path(cmps, *i))
		{
			free(free_if_error);
			free_canonical(cmps, pathname, ENOENT);
			return (FAILURE);
		}
		*i += 2;
	}
	while (cmps[*i] && ft_strncmp(cmps[*i], ".", 2) == 0)
		(*i)++;
	return (SUCCESS);
}

char	*canonical_path(char *pathname)
{
	char	**cmps;
	char	*result;
	int		i;

	i = -1;
	cmps = ft_split(pathname, '/');
	if (!cmps)
		return (NULL);
	result = NULL;
	while (cmps[++i])
	{
		if (skip_dot(cmps, pathname, &i, result) == FAILURE)
			return (NULL);
		if (!cmps[i])
			break ;
		if (update_path(&result, cmps[i]) == FAILURE)
		{
			free_canonical(cmps, pathname, 0);
			return (NULL);
		}
	}
	free_canonical(cmps, pathname, 0);
	return (result);
}
