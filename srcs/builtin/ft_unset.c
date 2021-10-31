#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static void	unset_env(char *av_i, t_env **env)
{
	if (identifier_type(av_i) != NOTHING)
	{
		ft_putstr_fd("minishell: unset: \'", STDERR_FILENO);
		ft_putstr_fd(av_i, STDERR_FILENO);
		ft_putendl_fd("\': not a valid identifier", STDERR_FILENO);
		g_sig_info.exit_status = EXIT_FAILURE;
	}
	else
	{
		g_sig_info.exit_status = EXIT_SUCCESS;
		delete_env(env, av_i);
	}
}

int	ft_unset(char **av)
{
	int		i;
	t_env	*env;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	i = 1;
	while (av[i])
	{
		unset_env(av[i], &env);
		i++;
	}
	if (list_to_environ(env) == FAILURE)
	{
		ft_envclear(&env, free);
		return (FAILURE);
	}
	ft_envclear(&env, free);
	return (SUCCESS);
}
