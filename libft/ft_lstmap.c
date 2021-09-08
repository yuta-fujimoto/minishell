/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/11 14:59:42 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/21 11:54:42 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*rlt;
	t_list	*tmp;

	rlt = NULL;
	while (lst)
	{
		tmp = malloc(sizeof(t_list));
		if (!tmp)
		{
			ft_lstclear(&rlt, del);
			return (NULL);
		}
		tmp->word = f(lst->word);
		tmp->next = NULL;
		ft_lstadd_back(&rlt, tmp);
		lst = lst->next;
	}
	return (rlt);
}
