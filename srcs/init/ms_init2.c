#include "../../incs/minishell.h"

static void	init_pwd(t_env *env, t_set *set)
{
	t_env	*pwd_env;

	pwd_env = ft_find_env_var(env, "PWD");
	if (!pwd_env)
	{
		if (!ft_envadd_back(&env, ft_envnew(ft_strdup("PWD"),
					get_current_directory())))
			ms_exit(set, EXIT_FAILURE, true);
		return ;
	}
	free(pwd_env->value);
	pwd_env->value = get_current_directory();
	if (!pwd_env->value)
		ms_exit(set, EXIT_FAILURE, true);
}

static void	init_shlvl(t_env *env, t_set *set)
{
	t_env	*shlvl_env;
	int		shlvl_int;

	shlvl_env = ft_find_env_var(env, "SHLVL");
	if (!shlvl_env)
	{
		if (!ft_envadd_back(&env, ft_envnew(ft_strdup("SHLVL"),
					ft_strdup("1"))))
			ms_exit(set, EXIT_FAILURE, true);
		return ;
	}
	shlvl_int = ft_atoi(shlvl_env->value);
	if (shlvl_int >= 999)
		shlvl_int = 1;
	else
		shlvl_int++;
	free(shlvl_env->value);
	shlvl_env->value = ft_itoa(shlvl_int);
	if (!shlvl_env->value)
		ms_exit(set, EXIT_FAILURE, true);
}

void	init_env(t_set *set)
{
	t_env		*env;
	extern char	**environ;

	set->safe_envrion = environ;
	env = environ_to_list();
	if (!env)
		ms_exit(set, EXIT_FAILURE, true);
	init_shlvl(env, set);
	init_pwd(env, set);
	if (list_to_environ(env) == FAILURE)
	{
		ft_envclear(&env, free);
		ms_exit(set, EXIT_FAILURE, true);
	}
	ft_envclear(&env, free);
}
