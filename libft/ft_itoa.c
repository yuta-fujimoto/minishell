/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 21:48:04 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/15 23:47:40 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	ctn_digit(int n)
{
	int	rlt;

	rlt = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		rlt++;
		n /= 10;
	}
	return (rlt);
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-1 * n);
	return (n);
}

static char	*get_zero(char *p)
{
	p[0] = '0';
	return (p);
}

char	*ft_itoa(int n)
{
	int		minus;
	int		tmp;
	char	*ptr;

	if (n < 0)
		minus = 1;
	else
		minus = 0;
	tmp = ctn_digit(n);
	ptr = malloc(sizeof(char) * (tmp + 1 + minus));
	if (!ptr)
		return (0);
	ptr[tmp + minus] = '\0';
	if (n == 0)
		return (get_zero(ptr));
	while (n != 0)
	{
		ptr[(--tmp) + minus] = ft_abs(n % 10) + '0';
		n /= 10;
	}
	if (minus)
		ptr[0] = '-';
	return (ptr);
}
