#include "../../incs/minishell.h"

static bool	add_lst_by_splited(t_list **t, char **split_rlt)
{
	int	i;

	i = 0;
	if (!split_rlt)
		return (false);
	while (split_rlt[i])
	{
		if (!ft_lstadd_back(t, ft_lstnew(split_rlt[i], STR)))
		{
			ft_free_str_arr(split_rlt);
			return (false);
		}
		i++;
	}
	free(split_rlt);
	return (true);
}

static int	add_lst(t_node *node, int i, t_list **lst)
{
	int	rlt;

	if (node->str_flgs[i] == STR_VAL)
		rlt = add_lst_by_splited(lst, ft_split(node->av[i], ' '));
	else
		rlt = ft_lstadd_back(lst,
				ft_lstnew(ft_strdup(node->av[i]), node->str_flgs[i]));
	if (!rlt)
	{
		ft_lstclear(lst, free);
		return (FAILURE);
	}
	return (SUCCESS);
}

static char	**list_to_argv(t_list *lst)
{
	int		size;
	int		i;
	char	**av;

	i = 0;
	size = ft_lstsize(lst);
	av = ft_calloc(sizeof(char *), size + 1);
	if (!av)
		return (NULL);
	while (lst)
	{
		av[i] = lst->word;
		i++;
		lst = lst->next;
	}
	return (av);
}

static int	reset_str_flgs(t_node *node, t_list *lst)
{
	int	i;

	i = 0;
	free(node->str_flgs);
	node->str_flgs = ft_calloc(sizeof(int), ft_lstsize(lst));
	if (!node->str_flgs)
		return (FAILURE);
	while (lst)
	{
		node->str_flgs[i] = lst->flags;
		lst = lst->next;
		i++;
	}
	return (SUCCESS);
}

int	split_argv_by_blank(t_node *node)
{
	t_list	*lst;
	int		i;
	int		rlt;

	i = 0;
	if (!node->av)
		return (SUCCESS);
	lst = NULL;
	while (node->av[i])
	{
		rlt = add_lst(node, i++, &lst);
		if (rlt == FAILURE)
			return (FAILURE);
	}
	if (reset_str_flgs(node, lst) == FAILURE)
		return (FAILURE);
	ft_free_str_arr(node->av);
	node->av = list_to_argv(lst);
	ft_lstclear(&lst, NULL);
	if (!node->av)
		return (FAILURE);
	return (SUCCESS);
}
