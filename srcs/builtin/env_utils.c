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

t_env	*environ_to_list(void)
{
	t_env		*env;
	extern char	**environ;
	char		**tmp;

	env = NULL;
	tmp = environ;
	while (*tmp)
	{
		if (!ft_envadd_back(&env,
			ft_envnew(get_name(*tmp), get_value(*tmp))))
		{
			ft_envclear(&env, free);
			return (NULL);
		}
		tmp++;
	}
	return (env);
}

char	**list_to_environ(t_env *env)
{
	int		env_len;
	char	**environ;
	int		i;

	env_len = ft_envsize(env);
	environ = ft_calloc(sizeof(char *), env_len + 1);
	if (!environ)
		return (NULL);
	i = 0;
	while (env)
	{
		environ[i] = ft_strcjoin(env->name, env->value, '=');
		if (!environ[i])
		{
			free_environ();
			return (NULL);
		}
		env = env->next;
		i++;
	}
	return (environ);
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
	free_str_arr(environ);
}
