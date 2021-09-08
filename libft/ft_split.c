/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 19:45:48 by fyuta             #+#    #+#             */
/*   Updated: 2021/04/21 11:55:34 by fyuta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(char const *str, char c)
{
	int	count;

	count = 0;
	if (*str == 0)
		return (0);
	if (*str != c)
		count++;
	while (*(str + 1))
	{
		if (*str == c && *(str + 1) != c)
			count++;
		str++;
	}
	return (count);
}

static int	str_cpy(char **dest, char const *src, int start, char const c)
{
	int	i;
	int	end;

	i = 0;
	end = start;
	while (src[end] != '\0' && src[end] != c)
		end++;
	*dest = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!(*dest))
		return (0);
	while (i + start < end)
	{
		(*dest)[i] = src[i + start];
		i++;
	}
	(*dest)[i] = '\0';
	return (end);
}

static void	*return_error(char **result, int j)
{
	int	i;

	i = -1;
	while (++i < j)
		free(result[i]);
	free(result);
	return (NULL);
}

char	**ft_split(char const *str, char c)
{
	char	**result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (count_word(str, c) + 1));
	if (!result)
		return (NULL);
	while (str[j])
	{
		if (str[j] != c)
		{
			j = str_cpy(&result[i++], str, j, c);
			if (!j)
				return (return_error(result, i));
		}
		if (str[j++] == '\0')
			break ;
	}
	result[i] = NULL;
	return (result);
}
