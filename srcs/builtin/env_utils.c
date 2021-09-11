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
		if (*environ)
		{
			ft_envadd_back(&env,
				ft_envnew(get_name(*environ), get_value(*environ)));
			environ++;
		}
		else
		{
			ft_envclear(&env, free);
			return (NULL);
		}
	}
	return (env);
}
