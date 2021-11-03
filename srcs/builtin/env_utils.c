#include "../../incs/minishell.h"

char	*get_name(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	return (ft_substr(s, 0, i));
}

char	*get_value(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (!s[i])
		return (NULL);
	return (ft_substr(s, i + 1, ft_strlen(s) - i));
}
