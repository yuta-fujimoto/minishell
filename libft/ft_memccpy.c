/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 12:01:24 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/14 14:17:29 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char		*ptr1;
	const unsigned char	*ptr2;
	unsigned char		uc;

	ptr1 = dest;
	ptr2 = src;
	uc = c;
	if (n == 0 || dest == src)
		return (NULL);
	while (n-- > 0)
	{
		*(ptr1++) = *(ptr2++);
		if (*(ptr1 - 1) == uc)
			return ((void *)ptr1);
	}
	return (NULL);
}
