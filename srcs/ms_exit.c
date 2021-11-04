#include "../incs/minishell.h"

void	ms_exit(t_set *set, int exit_status, bool exit_done)
{
	extern char	**environ;

	free_environ();
	environ = set->safe_envrion;
	if (exit_done)
	{
		rl_clear_history();
		free_set(set);
		if (!mod_termios_attr(set, false))
			exit(EXIT_FAILURE);
	}
	exit(exit_status);
}
