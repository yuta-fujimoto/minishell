#include "../../incs/minishell.h"

static int	expansion_conclude(t_env **env, char *free_s, int ret_status)
{
	if (env)
		ft_envclear(env, free);
	if (free_s)
		free(free_s);
	return (ret_status);
}

static bool	update_in_quote(t_exp *exp)
{
	if (exp->str[exp->i] == '\'' && !exp->in_dquote)
		exp->in_squote = !exp->in_squote;
	else if (exp->str[exp->i] == '\"' && !exp->in_squote)
		exp->in_dquote= !exp->in_dquote;
	else
		return (false);
	exp->i++;
	return (true);
}

static t_exp init_exp(char **str)
{
	t_exp exp;

	exp.str = *str;
	exp.in_dquote = false;
	exp.in_squote = false;
	exp.rlt_status = SUCCESS;
	exp.exp_str = NULL;
	exp.i = 0;
	return (exp);
}

int	expansion(char **str, t_env *env, bool *var_expansion)
{
	t_exp	exp;

	exp = init_exp(str);
	while (exp.str[exp.i])
	{
		if (update_in_quote(&exp) && !exp.in_squote && !exp.in_dquote)
			continue ;
		if (exp.in_squote || (exp.in_dquote && exp.str[exp.i] != '$' &&
			!(exp.str[exp.i] == '\\' && ft_strchr("$\'\"\\", exp.str[exp.i + 1]) != NULL)))
			exp.rlt_status = add_str_in_quote_to_word(&exp);
		else if (exp.str[exp.i] == '\\' && !exp.in_squote && exp.str[exp.i + 1] != '\0')
			exp.rlt_status = add_char_to_word(&exp, 2);
		else if (exp.str[exp.i] == '$' && exp.str[exp.i + 1] && !exp.in_squote)
		{
			exp.rlt_status = add_var_to_word(&exp, env);
			if (var_expansion != NULL && !exp.in_dquote)
				*var_expansion = true;
		}
		else
			exp.rlt_status = add_char_to_word(&exp, 1);
		if (exp.rlt_status == FAILURE)
			return (expansion_conclude(NULL, exp.exp_str, FAILURE));
	}
	*str = exp.exp_str;
	return (SUCCESS);
}

int	expansion_node(t_node *node)
{
	bool	var_expansion;
	t_env	*env;
	int		i;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	i = 0;
	var_expansion = false;
	while (node->av[i])
	{
		if (node->str_flgs[i] == STR && expansion(&node->av[i], env, &var_expansion) == FAILURE)
			return (expansion_conclude(&env, NULL, FAILURE));
		if (var_expansion)
			node->str_flgs[i] = STR_VAL;
		i++;
	}
	if (eliminate_null_node(node) == FAILURE || split_argv_by_blank(node) == FAILURE || list_to_environ(env) == FAILURE)
		return (expansion_conclude(&env, NULL, FAILURE));
	return (expansion_conclude(&env, NULL, SUCCESS));
}

void	expansion_node_conclude(t_node *node)
{
	int	i;

	i = 0;
	while (node->av[i])
	{
		free(node->av[i]);
		i++;
	}
	free(node->av);
}
