#include "../incs/minishell.h"

static bool	close_heredocs(t_set *set)
{
	while(set->heredoc_lst)
	{	
		close(set->heredoc_lst->fds[0]);
		close(set->heredoc_lst->fds[1]);
		set->heredoc_lst = set->heredoc_lst->next;
	}
	return (FAILURE);
}

static bool	create_heredoc(char **av, t_set *set)
{
	int	fds[2];
	int	i;

	i = 0;
	if (pipe(fds) == SYS_ERROR)
		return (close_heredocs(set));
	if (fds[0] == _POSIX_OPEN_MAX || fds[1] == _POSIX_OPEN_MAX)
	{
		close(fds[0]);
		close(fds[1]);
		ft_putendl_fd("minishell: maximum here-document count exceeded", STDERR_FILENO);// connect with minishell error somehow
		return (close_heredocs(set));
	}
	while (!str_equal(av[i], "<<", 3))
		i++;
	if (!ft_doclstadd_back(&set->heredoc_lst, ft_doclstnew(fds, av[++i])))
		return (close_heredocs(set));
	return (SUCCESS);
}

static bool	open_heredocs(t_tree *l, t_set *set, int *rlt)
{
	if (*rlt == FAILURE)
		return (FAILURE);
	if (!l)
		return (SUCCESS);
	open_heredocs(l->left, set, rlt);
	if (l->node.av && has_heredoc(l->node.av))
	{
		if (create_heredoc(l->node.av, set) == FAILURE)
			return (minishell_error(NULL, rlt));
	}
	open_heredocs(l->right, set, rlt);
	return (*rlt);
}

static bool	write_heredocs(t_doclist *heredoc_lst)
{
	char 	*delim;
	bool	exp;

	while (heredoc_lst)
	{
		exp = false;
		delim = heredoc_lst->delim;//expand func and enter exp	
		if (!handle_heredoc(heredoc_lst->fds, delim, exp))
			return (false);
		heredoc_lst = heredoc_lst->next;
	}
	return (true);
}

bool	init_heredocs(t_tree *parent, t_set *set, int *rlt)
{
	if (open_heredocs(parent, set, rlt) == FAILURE)
		return (FAILURE);	
	if (!write_heredocs(set->heredoc_lst))
	{
		close_heredocs(set);
		return (minishell_error(NULL, rlt));
	}
	return (SUCCESS);
}//make sure to close heredocs after redirection... and all docs for that matter
