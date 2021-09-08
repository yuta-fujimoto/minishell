/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 12:51:05 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/21 11:55:07 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				slen;
	unsigned char	t;

	slen = (int)ft_strlen(s);
	t = c;
	while (slen >= 0)
	{
		if (s[slen] == t)
			return ((char *)&s[slen]);
		slen--;
	}
	return (NULL);
}
