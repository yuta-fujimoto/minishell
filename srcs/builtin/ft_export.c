#include "../../incs/minishell.h"

int	ft_export_end(t_env *env, int rlt)
{
	if (list_to_environ(env) == FAILURE)
		rlt = FAILURE;
	ft_envclear(&env, free);
	return (rlt);
}

bool	ft_export_print(t_env *env)
{
	t_env	*tmp;
	int		env_count;
	int		i;
	int		j;

	env_count = ft_envsize(env);
	tmp = env;
	i = 0;
	while (i < env_count)
	{
		j = 0;
		env = tmp;
		while (j + i < env_count - 1)
		{
			if (ft_strncmp(env->name, env->next->name, UINT_MAX) > 0)
				ft_swap_env(env, env->next);
			env = env->next;
			j++;
		}
		i++;
	}
	print_name_value(tmp);
	return (ft_export_end(env, SUCCESS));
}

bool	ft_export_update(char *s, t_env **env)
{
	char	*name;
	char	*value;
	t_env	*env_var;

	name = get_name(s);
	value = get_value(s);
	env_var = ft_find_env_var(*env, name);
	if (env_var && value)
	{
		free(name);
		free(env_var->value);
		env_var->value = value;
		return (SUCCESS);
	}
	return (ft_envadd_back(env, ft_envnew(name, value)));
}

bool	ft_export_add(char *s, t_env **env)
{
	t_env	*p;
	char	*tmp;
	char	*name;
	char	*value;

	name = get_name(s);
	value = get_value(s);
	p = ft_find_env_var(*env, name);
	if (!p)
		return (ft_envadd_back(env, ft_envnew(name, value)));
	tmp = p->value;
	p->value = ft_strjoin(tmp, value);
	free(value);
	free(name);
	free(tmp);
	if (!p->value)
		return (false);
	return (true);
}

int	ft_export(char **av)
{
	int			type;
	t_env		*env;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	if (!*(++av))
		return (ft_export_print(env));
	while (*av)
	{
		type = identifier_type(*av);
		if (type == ERROR)
			ft_export_error(*av);
		else if (type == UPDATE && !ft_export_update(*av, &env))
			return (ft_export_end(env, FAILURE));
		else if (type == ADD && !ft_export_add(*av, &env))
			return (ft_export_end(env, FAILURE));
		av++;
	}
	return (ft_export_end(env, SUCCESS));
}
