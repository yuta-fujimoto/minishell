#include "../incs/minishell.h"

int			fd;
t_sig_info	g_sig_info = {0, 0, false, NULL, false, false, false, false};

void	ft_printf(void *word)
{
	if (word)
		dprintf(fd, "[%s]\n", (char *)word);
}

void	free_set(t_set *set)
{
	ft_lstclear(&set->lst, free);
	free_tree(set->tree);
	set->tree = NULL;
	if (set->input)
	{
		free(set->input);
		set->input = NULL;
	}	
	if (set->heredoc_lst)
	{
		close_heredocs(set->heredoc_lst);
		ft_doclstclear(&set->heredoc_lst);
	}
	if (g_sig_info.sys_error)
	{
		mod_termios_attr(set, false);
		exit(EXIT_FAILURE);
	}
}

void	sigquit_handler(int sigquit)
{
	(void)sigquit;
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
		if (close(STDIN_FILENO) == SYS_ERROR)
			g_sig_info.sys_error = true;
		g_sig_info.heredoc = false;
	}
	else if (!g_sig_info.child)
	{
		if (!g_sig_info.heredoc_sigint && !g_sig_info.heredoc_sigeof)
			write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (g_sig_info.child)
		write(STDOUT_FILENO, "\n", 1);
}

static void	init_sig_handler(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		exit(EXIT_FAILURE);
	if (signal(SIGQUIT, sigquit_handler) == SIG_ERR)
		exit(EXIT_FAILURE);
}

static void	handle_sigint(t_set *set)
{
	if (g_sig_info.term_stdin)
	{
		if (open(g_sig_info.term_stdin, O_RDONLY) == SYS_ERROR)
		{
			mod_termios_attr(set, false);
			exit(EXIT_FAILURE);
		}
		g_sig_info.term_stdin = NULL;
	}
	g_sig_info.signal = 0;
}

bool	mod_termios_attr(t_set *set, int init)
{
	unsigned int	lflag;
	unsigned char	vquit;

	if (init)
	{
		lflag = C_LFLAGS;
		vquit = false;
	}
	else
	{
		lflag = set->safe_c_lflag;
		vquit = set->safe_c_vquit;
	}
	set->t.c_lflag = lflag;
	set->t.c_cc[VQUIT] = vquit;
	if (isatty(STDIN_FILENO) && tcsetattr(STDIN_FILENO, TCSANOW, &set->t) == SYS_ERROR)
	{
		perror(NULL);
		g_sig_info.sys_error = true;
		return (false);
	}
	return (true);
}

static void	init_termios_attr(t_set *set)
{
	g_sig_info.exit_status = EXIT_SUCCESS;
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &set->t) == SYS_ERROR)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	set->safe_c_lflag = set->t.c_lflag;
	set->safe_c_vquit = set->t.c_cc[VQUIT];
	if (!mod_termios_attr(set, true))
		exit(EXIT_FAILURE);
}

int	main(int ac, char **av)
{
	t_set	set;
	int		rlt;
	bool	is_not_syntax_error;

	(void)ac;
	(void)av;
	fd = open("result.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH);
	init_sig_handler();
	init_termios_attr(&set);
	set.heredoc_lst = NULL;
	while (1)
	{
		rlt = SUCCESS;
		set.exit_done = false;
		handle_sigint(&set);
		g_sig_info.child = false;
		set.input = readline("minishell > ");
		g_sig_info.heredoc_sigint = false;
		g_sig_info.heredoc_sigeof = false;
		if (!set.input)
		{	
			ft_putstr_fd("\033[Aminishell > ", STDOUT_FILENO);
			ft_putendl_fd("exit", STDERR_FILENO);
			mod_termios_attr(&set, false);
			exit(g_sig_info.exit_status);
		}
		lexar(&set);
		dprintf(fd, "\ninput >> %s\n", set.input);
		dprintf(fd, "\n====result of lexar====\n");
		if (set.lst)
			ft_lstiter(set.lst, ft_printf);
		dprintf(fd, "\n====result of parser====\n");
		is_not_syntax_error = parser(&set);
		if (*set.input)
			add_history(set.input);
		if (is_not_syntax_error)
		{	
			init_heredocs(set.tree, &set, &rlt);
			if (rlt == SUCCESS)
				execute_input(set.tree, &set, &rlt);
			free_set(&set);
			if (rlt == FAILURE && !g_sig_info.signal)
			{
				mod_termios_attr(&set, false);
				exit(EXIT_FAILURE);
			}
		}
		else
			free_set(&set);
	}
}
