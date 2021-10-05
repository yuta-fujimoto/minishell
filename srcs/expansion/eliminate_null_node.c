#include "../../incs/minishell.h"

static t_list *extract_null_node(t_list **lst, t_list *prev, t_list *p)
{
	t_list *null_node;

	if (prev)
	{
		null_node = p;
		prev->next = p->next;
	}
	else
	{
		null_node = *lst;
		*lst = (*lst)->next;
	}
	return (null_node);
}

void	eliminate_null_node(t_list **lst)
{
	t_list	*null_node;
	t_list	*p;
	t_list	*prev;

	p = *lst;
	prev = NULL;
	while (p)
	{
		if (!p->word)
		{
			null_node = extract_null_node(lst, prev, p);
			p = p->next;
			ft_lstdelone(null_node, free);
			continue ;
		}
		prev = p;
		p = p->next;
	}
}
