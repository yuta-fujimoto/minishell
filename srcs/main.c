#include "../incs/minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

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

int	main(int ac, char **av)
{
	t_set	set;
	bool	is_not_syntax_error;

	(void)ac;
	init_params(&set);
	ms_init(&set);
	if (str_equal("-c", av[1], 3))
		set.input = ft_strdup(av[2]);
	else
		set.input = readline("\033[38;5;75mmsh-0.42$\033[39m ");
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
		if (str_equal("-c", av[1], 3))
		{
			mod_termios_attr(&set, false);
			exit(g_sig_info.exit_status);
		}
		init_params(&set);
		set.input = readline("\033[38;5;33mmsh-0.42$\033[39m ");
	}
}
