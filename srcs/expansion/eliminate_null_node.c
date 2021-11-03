#include "../../incs/minishell.h"
#include <unistd.h>

extern t_sig_info	g_sig_info;

static void	free_node_av(t_node *node)
{
	int	i;

	i = 0;
	while (i < node->ac)
	{
		if (node->av[i])
			free(node->av[i]);
		i++;
	}
	free(node->av);
}

static int	get_new_av_size(t_node *node)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (i < node->ac)
	{
		if (node->av[i] != NULL)
			size++;
		i++;
	}
	return (size);
}

static bool	has_ambiguous_redirect(t_node *exp_node, t_node *node)
{
	int	i;

	i = 1;
	while (i < exp_node->ac)
	{
		if (exp_node->av[i] == NULL && is_rdir(exp_node->str_flgs[i - 1]))
		{
			g_sig_info.exit_status = EXIT_FAILURE;
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->av[i], STDERR_FILENO);
			ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
			return (true);
		}
		i++;
	}
	return (false);
}

static void	set_new_av_strflgs(t_node *node, char **new_av, int new_size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < new_size)
	{
		if (node->av[i] != NULL)
		{
			new_av[j] = node->av[i];
			node->str_flgs[j++] = node->str_flgs[i];
		}
		i++;
	}
	free(node->av);
	node->av = new_av;
}

int	eliminate_null_node(t_node *exp_node, t_node *node)
{
	char	**new_av;
	int		new_size;

	new_size = get_new_av_size(exp_node);
	if (new_size == 0 || has_ambiguous_redirect(exp_node, node))
	{
		free_node_av(exp_node);
		exp_node->av = NULL;
		return (SUCCESS);
	}
	new_av = ft_calloc(sizeof(char *), new_size + 1);
	if (!new_av)
		return (FAILURE);
	set_new_av_strflgs(exp_node, new_av, new_size);
	exp_node->ac = new_size;
	return (SUCCESS);
}
