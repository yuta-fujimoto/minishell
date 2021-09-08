/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 12:06:11 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/12 19:23:20 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ucs;
	unsigned char	*ucd;
	unsigned char	*t;

	if (n == 0 || dest == src)
		return (dest);
	ucs = (unsigned char *)src;
	ucd = (unsigned char *)dest;
	if (ucd < ucs)
	{
		t = ucs + n;
		while (ucs < t)
			*(ucd++) = *(ucs++);
	}
	else
	{
		t = ucs;
		ucd += n;
		ucs += n;
		while (ucs > t)
			*(--ucd) = *(--ucs);
	}
	return (dest);
}
