/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/11 13:01:18 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/12 19:05:46 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (lst == NULL || new == NULL)
		return (false);
	if (*lst == NULL)
	{
		(*lst) = new;
		return (true);
	}
	tmp = *lst;
	while ((*lst)->next)
		(*lst) = (*lst)->next;
	(*lst)->next = new;
	*lst = tmp;
	return (true);
}
