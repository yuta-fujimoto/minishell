#include "../incs/minishell.h"

int			fd;
t_sig_info	g_sig_info = {0, false, NULL, false, false};

void	ft_printf(void *word)
{
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
}

void	sigint_handler(int sigint)
{
	g_sig_info.signal = sigint;
	if (g_sig_info.heredoc)
	{	
		write(STDOUT_FILENO, "\n", 1);
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

static void	init_sig_handlers(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
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

void	mod_termios_attr(t_set *set, int init)
{
	unsigned int	lflag;
	int				vquit;

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
	if (tcsetattr(STDIN_FILENO, TCSANOW, &set->t) == SYS_ERROR)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}
}

static void	init_termios_attr(t_set *set)
{
	if (tcgetattr(STDIN_FILENO, &set->t) == SYS_ERROR)
	{
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	set->safe_c_lflag = set->t.c_lflag;
	set->safe_c_vquit = set->t.c_cc[VQUIT];
	mod_termios_attr(set, true);
}

int	main(int ac, char **av)
{
	t_set	set;
	int		ret;	

	(void)ac;
	(void)av;
	fd = open("result.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH);
	init_sig_handlers();
	init_termios_attr(&set);
	while (1)
	{	
		handle_sigint(&set);
		set.input = readline("minishell > ");
		g_sig_info.heredoc_sigint = false;
		g_sig_info.heredoc_sigeof = false;
		if (!set.input)
		{
			mod_termios_attr(&set, false);
			write(STDOUT_FILENO, "exit\n", 5);
			exit(EXIT_SUCCESS);
		}
		set.lst = lexar(set.input);
		dprintf(fd, "\ninput >> %s\n", set.input);
		dprintf(fd, "\n====result of lexar====\n");
		ft_lstiter(set.lst, ft_printf);
		dprintf(fd, "\n====result of parser====\n");
		set.tree = parser(set.lst);
		if (*set.input)
			add_history(set.input);
		ret = execute_input(set.tree, &set);
		free_set(&set);
		if (ret == FAILURE && !g_sig_info.signal)
		{
			mod_termios_attr(&set, false);
			exit(EXIT_FAILURE);
		}
	}
}
