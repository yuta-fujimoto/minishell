#include "../incs/minishell.h"

t_sig_info	g_sig_info = {0, false, NULL, false, false, 0};

static void	handle_signal(t_set *set)
{
	if (g_sig_info.term_stdin)
	{
		if (open(g_sig_info.term_stdin, O_RDONLY) == SYS_ERROR)
			ms_exit(set, EXIT_FAILURE);
		g_sig_info.term_stdin = NULL;
	}
	g_sig_info.signal = 0;
	g_sig_info.heredoc_sigint = false;
	g_sig_info.heredoc_sigeof = false;
}

void	ms_execution(t_set *set)
{
	int	ret;

	ret = execute_input(set->tree, set);
	free_set(set);
	if (ret == FAILURE && !g_sig_info.signal)
		ms_exit(set, EXIT_FAILURE);
}

void	ms_exit_eof(t_set *set)
{
	ft_putstr_fd("\033[Aminishell > ", STDOUT_FILENO);
	ft_putendl_fd("exit", STDERR_FILENO);
	ms_exit(set, g_sig_info.exit_status);
}

int	main()
{
	t_set	set;
	bool	is_not_syntax_error;

	set.input = readline("minishell > ");
	ms_init(&set);
	while (1)
	{
		if (!set.input)
			ms_exit_eof(&set);
		if (*set.input)
			add_history(set.input);
		handle_signal(&set);
		lexar(&set);
		is_not_syntax_error = parser(&set);
		if (is_not_syntax_error)
			ms_execution(&set);
		else
			free_set(&set);
		set.input = readline("minishell > ");
	}
}
