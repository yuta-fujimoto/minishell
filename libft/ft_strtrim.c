/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 17:21:52 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/21 11:58:08 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strlen_for_trim(const char *s)
{
	size_t	i;

	i = 0;
	while (*(s++))
		i++;
	if (i == 0)
		return (1);
	return (i);
}

static int	in_set(char const c, char const *set)
{
	while (*set)
	{
		if (c == *(set++))
			return (1);
	}
	return (0);
}

static char	*return_empty(void)
{
	char	*p;

	p = malloc(sizeof(char));
	if (!p)
		return (NULL);
	p[0] = 0;
	return (p);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	sidx;
	size_t	eidx;
	size_t	i;
	char	*ptr;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(set));
	i = -1;
	sidx = 0;
	eidx = ft_strlen_for_trim(s1) - 1;
	while (in_set(s1[sidx], set))
		sidx++;
	while (in_set(s1[eidx], set) && eidx > 0)
		eidx--;
	if (eidx < sidx)
		return (return_empty());
	ptr = malloc(sizeof(char) * (eidx - sidx + 2));
	if (!ptr)
		return (0);
	while (++i + sidx <= eidx)
		ptr[i] = s1[i + sidx];
	ptr[i] = 0;
	return (ptr);
}
