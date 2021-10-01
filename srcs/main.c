#include "../incs/minishell.h"

int	fd;

void	ft_printf(void *word)
{
	dprintf(fd, "[%s]\n", (char *)word);
}

void	sigint_handler(int signal)
{
	(void)signal;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_redisplay();
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

void	sigquit_handler(int signal)
{
	(void)signal;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av)
{
	t_set	set;
	int		ret;

	(void)ac;
	(void)av;
	fd = open("result.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH);
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		return (EXIT_SUCCESS);
	signal(SIGQUIT, sigquit_handler);
	while (1)
	{
		set.input = readline("minishell > ");
		if (!set.input)
		{
			if (!set.input)
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
		if (ret == FAILURE)
			exit(EXIT_FAILURE);
	}
}
