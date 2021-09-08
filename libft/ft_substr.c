/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 16:26:03 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/21 11:54:54 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*empty(void)
{
	char	*ptr;

	ptr = (char *)malloc(sizeof(char));
	ptr[0] = '\0';
	return (ptr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			slen;
	size_t			size;
	char			*ptr;
	unsigned int	i;

	if (s == NULL)
		return (NULL);
	slen = ft_strlen(s);
	i = -1;
	if (start >= (unsigned int)slen)
		return (empty());
	if (start + len >= slen)
		size = slen - start + 1;
	else
		size = len + 1;
	ptr = (char *)malloc(sizeof(char) * size);
	if (!ptr)
		return (NULL);
	while (++i < size - 1)
		ptr[i] = s[i + start];
	ptr[i] = 0;
	return (ptr);
}
