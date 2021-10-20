#include "../../incs/minishell.h"

static int	count_cmd_info(t_node *node)
{
	int	i;
	int	len;

	i = 0;
	len = ft_str_arr_len(node->av);
	while (node->av[i])
	{
		if (is_rdir(node->str_flgs[i]))
			len -= 2;
		i++;
	}
	return (len);
}

char	**create_new_cmd(t_node *node, bool *touch)
{
	int		i;
	int		j;
	int		new_cmd_len;
	char	**new_cmd;

	i = 0;
	j = 0;
	new_cmd_len = count_cmd_info(node);
	if (!new_cmd_len)
	{
		*touch = true;
		return (NULL);
	}
	new_cmd = malloc(sizeof(char *) * (new_cmd_len + 1));
	if (!new_cmd)
		return (NULL);
	while (node->av[i])
	{
		if (is_rdir(node->str_flgs[i]))
			i += 2;/*this could be trouble, ensure if > is not followed by anything, it is syntax error*/
		else
			new_cmd[j++] = node->av[i++];
	}
	new_cmd[j] = NULL;
	return (new_cmd);
}
