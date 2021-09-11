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
// I don't know how each setences in this function works.

void	ft_free(t_list **lst, t_tree **tree, char **input)
{
	ft_lstclear(lst, free);
	free_tree(*tree);
	*tree = NULL;
	if (input)
	{
		free(*input);
		*input = NULL;
	}
}

int	main(int ac, char **av)
{
	char	*input;
	t_list	*lst;
	t_tree	*tree;
	int		ret;

	(void)ac;
	(void)av;
	fd = open("result.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH);
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		return (EXIT_SUCCESS);
	while (1)
	{
		input = readline("minishell > ");
		if (!input || ft_strncmp(input, "exit", 5) == 0)
		{
			if (!input)
				write(STDOUT_FILENO, "exit\n", 5);
			else
				free(input);
			exit(EXIT_SUCCESS);
		}
		lst = lexar(input);
		dprintf(fd, "\ninput >> %s\n", input);
		dprintf(fd, "\n====result of lexar====\n");
		ft_lstiter(lst, ft_printf);
		dprintf(fd, "\n====result of parser====\n");
		tree = parser(lst);
		if (*input)
			add_history(input);
		ret = execute_input(tree);
		ft_free(&lst, &tree, &input);
		if (ret == FAILURE)
			exit(EXIT_FAILURE);
	}
}
