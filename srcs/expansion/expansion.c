#include "../../incs/minishell.h"

int	expansion_conclude(t_env **env, char *free_s, int ret_status)
{
	if (env)
		ft_envclear(env, free);
	if (free_s)
		free(free_s);
	return (ret_status);
}

static bool	update_in_quote(t_exp *exp)
{
	if (exp->word[exp->i] == '\'' && !exp->in_dquote)
		exp->in_squote = !exp->in_squote;
	else if (exp->word[exp->i] == '\"' && !exp->in_squote)
		exp->in_dquote = !exp->in_dquote;
	else
		return (false);
	exp->i++;
	return (true);
}

static t_exp	init_exp(char **word)
{
	t_exp	exp;

	exp.word = *word;
	exp.in_dquote = false;
	exp.in_squote = false;
	exp.rlt_status = SUCCESS;
	exp.exp_word = NULL;
	exp.i = 0;
	return (exp);
}

int	expansion(char **word, t_env *env, bool *var_exp)
{
	t_exp	exp;

	exp = init_exp(word);
	while (exp.word[exp.i])
	{
		if (update_in_quote(&exp) && !exp.in_squote && !exp.in_dquote)
			continue ;
		add_to_word(&exp, var_exp, env);
		if (exp.rlt_status == FAILURE)
			return (expansion_conclude(NULL, exp.exp_word, FAILURE));
	}
	*word = exp.exp_word;
	return (SUCCESS);
}
