#include "../incs/minishell.h"

t_sig_info	g_sig_info = {0, 0, false, NULL, false, false, false, false};

static void	init_params(t_set *set)
{
	if (g_sig_info.term_stdin)
	{
		if (open(g_sig_info.term_stdin, O_RDONLY) == SYS_ERROR)
			ms_exit(set, EXIT_FAILURE, true);
		g_sig_info.term_stdin = NULL;
	}
	g_sig_info.signal = 0;
	g_sig_info.heredoc_sigint = false;
	g_sig_info.heredoc_sigeof = false;
	g_sig_info.child = false;
	set->exit_done = false;
	set->lst = NULL;
	set->tree = NULL;
	set->heredoc_lst = NULL;
}

static void	ms_execution(t_set *set)
{
	int	rlt;

	rlt = SUCCESS;
	init_heredocs(set->tree, set, &rlt);
	if (rlt == SUCCESS)
		execute_input(set->tree, set, &rlt);
	free_set(set);
	if (rlt == FAILURE && !g_sig_info.signal)
		ms_exit(set, EXIT_FAILURE, true);
}

void	ms_exit_eof(t_set *set)
{
	ft_putstr_fd("\033[Aminishell > ", STDOUT_FILENO);
	ft_putendl_fd("exit", STDERR_FILENO);
	ms_exit(set, g_sig_info.exit_status, true);
}

void	minishell(void)
{
	t_set	set;
	bool	is_not_syntax_error;

	init_params(&set);
	ms_init(&set);
	set.input = readline("minishell > ");
	init_env(&set);
	while (1)
	{
		if (!set.input)
			ms_exit_eof(&set);
		if (*set.input)
			add_history(set.input);
		lexar(&set);
		is_not_syntax_error = parser(&set);
		if (is_not_syntax_error)
			ms_execution(&set);
		else
			free_set(&set);
		init_params(&set);
		set.input = readline("minishell > ");
	}
}

void	minishell_c_option(char *cmd)
{
	t_set	set;
	bool	is_not_syntax_error;

	init_params(&set);
	ms_init(&set);
	set.input = ft_strdup(cmd);
	init_env(&set);
	if (!set.input)
		ms_exit_eof(&set);
	if (*set.input)
		add_history(set.input);
	lexar(&set);
	is_not_syntax_error = parser(&set);
	if (is_not_syntax_error)
		ms_execution(&set);
	else
		free_set(&set);
	exit (g_sig_info.exit_status);
}

int	main(int ac, char **av)
{
	(void)ac;
	if (str_equal(av[1], "-c", 3))
		minishell_c_option(av[2]);
	else
		minishell();
}
