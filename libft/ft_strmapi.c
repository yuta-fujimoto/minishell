/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 22:39:26 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/15 23:51:16 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned	int, char))
{
	unsigned int	slen;
	unsigned int	i;
	char			*ptr;

	if (s == NULL || f == NULL)
		return (NULL);
	slen = 0;
	i = -1;
	while (s[slen] != '\0')
		slen++;
	ptr = malloc(sizeof(char) * (slen + 1));
	if (!ptr)
		return (NULL);
	while (++i < slen)
		ptr[i] = f(i, s[i]);
	ptr[i] = '\0';
	return (ptr);
}
