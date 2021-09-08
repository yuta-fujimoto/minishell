/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 17:08:15 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/21 11:55:24 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*ptr;
	int			i;
	size_t		slen1;
	size_t		slen2;

	if (!s1 || !s2)
		return (NULL);
	slen1 = ft_strlen(s1);
	slen2 = ft_strlen(s2);
	ptr = (char *)malloc(sizeof(char) * (slen1 + slen2 + 1));
	if (!ptr)
		return (0);
	i = 0;
	while (*s1)
		ptr[i++] = *(s1++);
	while (*s2)
		ptr[i++] = *(s2++);
	ptr[i] = '\0';
	return (ptr);
}
