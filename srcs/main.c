#include "../incs/minishell.h"

int			fd;
t_sig_info	g_sig_info = {0, false, NULL};

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

void	sigquit_handler(int sigquit)
{
	(void)sigquit;
	write(STDOUT_FILENO, "minishell >   \b\b", 16);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler(int sigint)
{
	g_sig_info.signal = sigint;
	if (g_sig_info.heredoc)
	{	
		g_sig_info.term_stdin = ttyname(STDIN_FILENO);
		close(STDIN_FILENO);
		g_sig_info.heredoc = false;
		write(STDOUT_FILENO, ">   \n", 5);
	}
	else
	{
		write(STDOUT_FILENO, "minishell >   \n", 15);
		rl_on_new_line();
		rl_replace_line("", 0);	
		rl_redisplay();
	}
}

static void	init_sig_handlers(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		exit(EXIT_SUCCESS); //should be EXIT_FAILURE?
	if (signal(SIGQUIT, sigquit_handler) == SIG_ERR)
		exit(EXIT_SUCCESS); //should be EXIT_FAILURE?	
}

static void	handle_sigint(void)
{
	if (g_sig_info.term_stdin)
	{
		if (open(g_sig_info.term_stdin, O_RDONLY) == SYS_ERROR)
			exit(EXIT_FAILURE);
		g_sig_info.term_stdin = NULL;
	}
	g_sig_info.signal = 0;
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
	while (1)
	{
		handle_sigint();
		set.input = readline("minishell > ");
		if (!set.input)
		{
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
			exit(EXIT_FAILURE);
	}
}
