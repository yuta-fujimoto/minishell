#include "../../incs/minishell.h"

bool	needs_removal(t_exp *exp)
{
	return (!exp->in_squote && exp->word[exp->i + 1]);
}

bool	is_word_in_dquote(t_exp *exp)
{
	return (exp->word[exp->i] != '$'
		&& !(exp->word[exp->i] == '\\' 
		&& ft_strchr("$\'\"\\", exp->word[exp->i + 1])));
}
