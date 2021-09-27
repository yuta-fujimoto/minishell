#include "../../incs/minishell.h"

int ft_cd_end(char *tmp, t_env **env, int ret_val)
{
	fflush(NULL);
	if (tmp)
		free(tmp);
	if (env)
		ft_envclear(env, free);
	return (ret_val);
}

int	cd_error(char *pathname)
{
	char	*err;

	err = ft_strjoin("minishell: cd ", pathname);
	if (!err)
		return (FAILURE);
	perror(err);
	free(err);
	return (SUCCESS);
}

char	*update_environ_value(t_env *env, char *value)
{
	char	*oldvalue;

	if (!env || !value)
		return (NULL);
	oldvalue = env->value;
	env->value = value;
	return (oldvalue);
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
		return (ft_cd_end(tmp, &env, FAILURE));
	free(environ);
	environ = list_to_environ(env);
	if (!environ)
		return (ft_cd_end(tmp, &env, FAILURE));
	return (ft_cd_end(tmp, &env, SUCCESS));
}
