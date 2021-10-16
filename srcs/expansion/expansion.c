#include "../../incs/minishell.h"

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

int	expansion(char **exp_word, char **word, t_env *env, bool *var_exp)
{
	t_exp	exp;

	exp = init_exp(word);
	while (exp.word[exp.i])
	{
		if (update_in_quote(&exp) && !exp.in_squote && !exp.in_dquote)
			continue ;
		add_to_word(&exp, var_exp, env);
		if (exp.rlt_status == FAILURE)
		{
			free(exp.exp_word);
			return (FAILURE);
		}
	}
	*exp_word = exp.exp_word;
	return (SUCCESS);
}
