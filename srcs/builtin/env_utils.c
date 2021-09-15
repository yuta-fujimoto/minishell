#include "../../incs/minishell.h"

char	*get_name(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	return (ft_substr(s, 0, i));
}

char	*get_value(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (ft_substr(s, i + 1, ft_strlen(s) - i));
}

t_env	*environ_to_list(char **environ)
{
	t_env		*env;

	env = NULL;
	while (*environ)
	{
		if (!ft_envadd_back(&env,
			ft_envnew(get_name(*environ), get_value(*environ))))
		{
			ft_envclear(&env, free);
			return (NULL);
		}
		environ++;
	}
	return (env);
}

char	**list_to_environ(t_env *env)
{
	int		env_len;
	char	**environ;
	int		i;

	env_len = ft_envsize(env);
	environ = ft_calloc(sizeof(char *), env_len);	
	if (!environ)
		return (NULL);
	i = 0;
	while (env)
	{
		environ[i] = ft_strcjoin(env->name, env->value, '=');
		if (!environ[i])
		{
			free_environ(environ);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	return (environ);
}

void	free_environ(char **environ)
{
	int			i;
	static bool	flg = false;

	i = 0;
	if (!environ || !flg)
	{
		flg = true;
		return ;
	}
	while (environ[i])
	{
		free(environ[i]);
		i++;
	}
	free(environ);
	*environ = NULL;
}
