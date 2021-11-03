#include "../../incs/minishell.h"

t_env	*ft_envnew_for_value_null(char *name, char *value)
{
	t_env	*new;

	if (!name)
	{
		if (name)
			free(name);
		return (NULL);
	}
	new = malloc(sizeof(t_env));
	if (!new)
	{
		free(name);
		free(value);
		return (NULL);
	}
	new->name = name;
	new->value = value;
	new->next = NULL;
	return (new);
}

t_env	*environ_to_list(void)
{
	t_env		*env;
	extern char	**environ;
	char		**tmp;
	char		*value;

	env = NULL;
	tmp = environ;
	while (*tmp)
	{
		value = get_value(*tmp);
		if ((!value && ft_strchr(*tmp, '='))
			|| !ft_envadd_back(&env,
				ft_envnew_for_value_null(get_name(*tmp), value)))
		{
			ft_envclear(&env, free);
			return (NULL);
		}
		tmp++;
	}
	return (env);
}

int	list_to_environ(t_env *env)
{
	char		**new_environ;
	extern char	**environ;
	int			i;

	new_environ = ft_calloc(sizeof(char *), ft_envsize(env + 1));
	if (!new_environ)
		return (FAILURE);
	i = 0;
	while (env)
	{
		if (env->value)
			new_environ[i] = ft_strcjoin(env->name, env->value, '=');
		else
			new_environ[i] = ft_strdup(env->name);
		if (!new_environ[i])
		{
			ft_free_str_arr(new_environ);
			return (FAILURE);
		}
		env = env->next;
		i++;
	}
	free_environ();
	environ = new_environ;
	return (SUCCESS);
}

void	free_environ(void)
{
	static bool	flg = false;
	extern char	**environ;

	if (!environ || !flg)
	{
		flg = true;
		return ;
	}
	ft_free_str_arr(environ);
}
