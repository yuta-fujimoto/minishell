/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 13:25:08 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/10 15:46:31 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	int				i;
	unsigned char	*uc1;
	unsigned char	*uc2;

	if (n == 0)
		return (0);
	i = 0;
	uc1 = (unsigned char *)s1;
	uc2 = (unsigned char *)s2;
	while (--n != 0 && uc1[i] != '\0' && uc2[i] != '\0')
	{
		if (uc1[i] != uc2[i])
			return (uc1[i] - uc2[i]);
		i++;
	}
	return (uc1[i] - uc2[i]);
}
