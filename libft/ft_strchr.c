/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 12:47:59 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/15 23:43:08 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	unsigned char	t;

	t = c;
	if (s == NULL)
		return (NULL);
	if (*s == t)
		return ((char *)s);
	s++;
	while (*(s - 1))
	{
		if (*s == t)
			return ((char *)s);
		s++;
	}
	return (NULL);
}
