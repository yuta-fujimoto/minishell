/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 12:22:55 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/16 15:36:55 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*src;
	unsigned char	t;
	size_t			i;

	t = c;
	src = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (src[i] == t)
			return ((void *)&src[i]);
		i++;
	}
	return (NULL);
}
