#include "../../incs/minishell.h"

char *update_environ_value(t_env *env, char *value)
{
	char *oldvalue;

	if (!env || !value)
		return (NULL);
	oldvalue = env->value;
	env->value = value;
	return (oldvalue);
}

char *update_path(char **pathname, char *newcmp)
{
	char *tmp;

	if (!newcmp)
		return (*pathname);
	if (*pathname)
	{
		tmp = *pathname;
		*pathname = ft_strcjoin(*pathname, newcmp, '/');
		free(tmp);
	}
	else
		*pathname = ft_strjoin("/", newcmp);
	return (*pathname);
}

char *absolute_path(char *pathname)
{
	char *current;
	char *rlt;

	current = get_current_directory();
	if (!current)
		return (NULL);
	if (current[ft_strlen(current) - 1] != '/')
		rlt = ft_strcjoin(current, pathname, '/');
	else
		rlt = ft_strjoin(current, pathname);
	free(current);
	return (rlt);
}

bool is_valid_path(char **cmps, int end)
{
	char		*pathname;
	struct stat	st;
	int			i;

	i = 0;
	pathname = NULL;
	while (i <= end)
	{
		if (!update_path(&pathname, cmps[i]))
			return (false);
		i++;
	}
	printf("%s\n", pathname);
	if (stat(pathname, &st) == -1 || ((st.st_mode & S_IFMT) != S_IFLNK
		&& (st.st_mode & S_IFMT) != S_IFDIR))
	{
		free(pathname);
		return (false);
	}
	free(pathname);
	return (true);
}

char	*canonical_path(char *pathname)
{
	char	**cmps;
	char	*result;
	int		i;

	i = 0;
	cmps = ft_split(pathname, '/');
	if (!cmps)
		return (NULL);
	result = NULL;
	while (cmps[i])
	{
		while (cmps[i] && cmps[i + 1] && ft_strncmp(cmps[i + 1], "..", 3) == 0 &&
				ft_strncmp(cmps[i], "..", 3) != 0)
		{
			if (!is_valid_path(cmps, i))
			{
				errno = ENOENT;
				free_str_arr(cmps);
				return (NULL);
			}
			i += 2;
		}
		while (cmps[i] && ft_strncmp(cmps[i], ".", 2) == 0)
			i++;
		if (!cmps[i])
			break ;
		if (update_path(&result, cmps[i]) == NULL)
		{
			free_str_arr(cmps);
			return (NULL);
		}
		i++;
	}
	if (!result)
		return (ft_strdup("/"));
	return (result);
}

int	set_working_directory(char *pathname)
{
	t_env *env;
	char *oldpwd;
	extern char **environ;

	env = environ_to_list();
	oldpwd = update_environ_value(ft_find_env_var(env, "PWD"),
			pathname);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	if (!update_environ_value(ft_find_env_var(env, "OLDPWD"), oldpwd))
		if (!ft_envadd_back(&env, ft_envnew(ft_strdup("OLDPWD"), oldpwd)))
			return (FAILURE);
	free_environ();
	environ = list_to_environ(env);
	ft_envclear(&env, free);
	return (SUCCESS);
}
