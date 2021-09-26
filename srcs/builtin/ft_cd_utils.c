#include "../../incs/minishell.h"

char	*update_environ_value(t_env *env, char *value)
{
	char	*oldvalue;

	if (!env || !value)
		return (NULL);
	oldvalue = env->value;
	env->value = value;
	return (oldvalue);
}

char	*update_path(char **pathname, char *newcmp)
{
	char	*tmp;

	if (!newcmp)
		return (*pathname);
	if (!(*pathname))
		return (ft_strjoin("/", newcmp));
	tmp = *pathname;
	if (*pathname[ft_strlen(*pathname) - 1] == '/')
		*pathname = ft_strjoin(*pathname, newcmp);
	else
		*pathname = ft_strcjoin(*pathname, newcmp, '/');
	free(tmp);
	return (*pathname);
}

char	*absolute_path(char *pathname)
{
	char	*current;
	char	*rlt;

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

int	set_working_directory(char *pathname)
{
	t_env		*env;
	char		*oldpwd;
	extern char	**environ;
	char		*tmp;

	env = environ_to_list();
	oldpwd = update_environ_value(ft_find_env_var(env, "PWD"),
			pathname);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	tmp = update_environ_value(ft_find_env_var(env, "OLDPWD"), oldpwd);
	if (!tmp && !ft_envadd_back(&env, ft_envnew(ft_strdup("OLDPWD"), oldpwd)))
		return (FAILURE);
	free(tmp);
	free_environ();
	environ = list_to_environ(env);
	ft_envclear(&env, free);
	return (SUCCESS);
}
