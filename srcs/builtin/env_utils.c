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

int	list_to_environ(t_env *env)
{
	int			env_len;
	char		**new_environ;
	extern char	**environ;
	int			i;

	env_len = ft_envsize(env);
	new_environ = ft_calloc(sizeof(char *), env_len + 1);
	if (!new_environ)
		return (FAILURE);
	i = 0;
	while (env)
	{
		new_environ[i] = ft_strcjoin(env->name, env->value, '=');
		if (!new_environ[i])
		{
			free_str_arr(new_environ);
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
	free_str_arr(environ);
}
