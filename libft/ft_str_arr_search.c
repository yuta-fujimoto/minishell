#include "../incs/minishell.h"

int	ft_str_arr_search(char **str_arr, char *search)
{
	int	i;

	i = 0;
	while (str_arr[i])
	{
		if (ft_strncmp(str_arr[i], search, ft_strlen(search)) == 0)
			return (i);
		i++;
	}
	return (-1);
}
