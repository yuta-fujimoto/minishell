#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static void	init_termios_attr(t_set *set)
{
	g_sig_info.exit_status = EXIT_SUCCESS;
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &set->t) == SYS_ERROR)
	{
		perror(NULL);
		free_environ();
		exit(EXIT_FAILURE);
	}
	set->safe_c_lflag = set->t.c_lflag;
	set->safe_c_vquit = set->t.c_cc[VQUIT];
	mod_termios_attr(set, true);
}

void	sigint_handler(int sigint)
{
	g_sig_info.exit_status = 1;
	g_sig_info.signal = sigint;
	if (g_sig_info.heredoc)
	{
		write(STDOUT_FILENO, "\n", 1);
		if (isatty(STDIN_FILENO))
			g_sig_info.term_stdin = ttyname(STDIN_FILENO);
		close(STDIN_FILENO);
		g_sig_info.heredoc = false;
	}
	else
	{
		if (!g_sig_info.heredoc_sigint && !g_sig_info.heredoc_sigeof)
			write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	init_sig_handler(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		exit(EXIT_FAILURE);
}

void	ms_init(t_set *set)
{
	init_sig_handler();
	init_env(set);
	init_termios_attr(set);
}
