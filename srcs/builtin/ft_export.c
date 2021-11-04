#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

int	ft_export_end(t_env *env, int rlt, int type)
{
	if (list_to_environ(env) == FAILURE)
		rlt = FAILURE;
	ft_envclear(&env, free);
	if (type == ERROR)
		g_sig_info.exit_status = EXIT_FAILURE;
	else
		g_sig_info.exit_status = EXIT_SUCCESS;
	return (rlt);
}

int	ft_export_print(t_env *env)
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
	return (ft_export_end(env, SUCCESS, NOTHING));
}

int	ft_export_update(char *name, char *value, t_env **env, int type)
{
	t_env	*env_var;

	if (!name || (!value && type != NOTHING))
	{
		if (name)
			free(name);
		if (value)
			free(value);
		return (FAILURE);
	}
	env_var = ft_find_env_var(*env, name);
	if (env_var && value)
	{
		free(name);
		free(env_var->value);
		env_var->value = value;
		return (SUCCESS);
	}
	if (env_var && !value)
		free(name);
	else if (!ft_envadd_back(env
			, ft_envnew_for_value_null(name, value)))
		return (FAILURE);
	return (SUCCESS);
}

int	ft_export_add(char *name, char *value, t_env **env)
{
	t_env	*p;
	char	*tmp;

	p = ft_find_env_var(*env, name);
	if (!p)
	{
		if (ft_envadd_back(env, ft_envnew(name, value)))
			return (SUCCESS);
		return (FAILURE);
	}
	tmp = p->value;
	p->value = ft_strjoin(tmp, value);
	free(value);
	free(name);
	free(tmp);
	if (!p->value)
		return (FAILURE);
	return (SUCCESS);
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
		else if ((type == UPDATE || type == NOTHING)
			&& ft_export_update(get_name(*av), get_value(*av),
				&env, type) == FAILURE)
			return (ft_export_end(env, FAILURE, type));
		else if (type == ADD && ft_export_add(get_name(*av),
			get_value(*av), &env) == FAILURE)
			return (ft_export_end(env, FAILURE, type));
		av++;
	}
	return (ft_export_end(env, SUCCESS, type));
}
