#include "../../incs/minishell.h"

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
	return (true);
}

bool	ft_export_update(char *s, t_env **env)
{
	char	*name;

	name = get_name(s);
	if (ft_find_env_var(*env, name))
		delete_env(env, name);
	return (ft_envadd_back(env, ft_envnew(name, get_value(s))));
}

bool	ft_export_add(char *s, t_env **env)
{
	t_env	*p;
	char	*tmp;
	char	*name;
	char	*value;

	name = get_name(s);
	value = get_value(s);
	if (!name || !value)
	{
		if (name && !value)
			free(name);
		return (false);
	}
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

int	ft_export(char **av, t_env **env)
{
	int	type;
	int	rlt;

	if (!*(++av))
		return (ft_export_print(*env));
	while (*av)
	{
		rlt = SUCCESS;
		type = identifier_type(*av);
		if (type == ERROR)
		{
			ft_putendl_fd("not a valid identifier", STDOUT_FILENO);
			rlt = FAILURE;
		}
		else if (type == UPDATE && !ft_export_update(*av, env))
			return (FAILURE);
		else if (type == ADD && !ft_export_add(*av, env))
				return (FAILURE);
		av++;
	}
	return (rlt);
}

/* debug
gcc -Wall -Wextra -Werror srcs/builtin/ft_export.c srcs/builtin/env_utils.c  srcs/builtin/ft_export_utils.c libft/libft.a
*/

/*
int	main(int ac, char **av)
{
	t_env		*env;
	extern char	**environ;

	(void)ac;
	env = environ_to_list(environ);
	if (ft_export(av, &env) == SUCCESS)
		print_name_value(env);
	ft_envclear(&env, free);
}
*/
