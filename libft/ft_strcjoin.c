/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthompso <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 21:52:24 by jthompso          #+#    #+#             */
/*   Updated: 2021/09/10 21:54:33 by jthompso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char    *ft_strcjoin(char const *s1, char const *s2, int c)
{
    char        *ptr;
    int         i;
    size_t      slen1;
    size_t      slen2;

    if (!s1 || !s2)
        return (NULL);
    slen1 = ft_strlen(s1);
    slen2 = ft_strlen(s2);
    ptr = (char *)malloc(sizeof(char) * (slen1 + slen2 + 2));
    if (!ptr)
        return (0);
    i = 0;
    while (*s1)
        ptr[i++] = *(s1++);
	ptr[i++] = (unsigned char)c;
    while (*s2)
        ptr[i++] = *(s2++);
    ptr[i] = '\0';
    return (ptr);
}
