#include "../../incs/minishell.h"

bool	is_valid(t_exp *exp)
{
	return (!exp->in_squote && exp->word[exp->i + 1]);
}

bool	is_word_in_dquote(t_exp *exp)
{
	return (exp->word[exp->i] != '$'
		&& !(exp->word[exp->i] == '\\'
			&& ft_strchr("$\'\"\\", exp->word[exp->i + 1])));
}

char	*get_var_name(t_exp *exp)
{
	int		j;
	char	*var_name;

	j = 1;
	if (ft_isdigit(exp->word[exp->i + 1]) || exp->word[exp->i + 1] == '?')
		var_name = ft_substr(exp->word, exp->i + 1, 1);
	else
	{
		while (exp->word[exp->i + j]
			&& ft_strchr("\'\"+= $", exp->word[exp->i + j]) == NULL)
			j++;
		var_name = ft_substr(exp->word, exp->i + 1, j - 1);
	}
	if (!var_name)
		return (NULL);
	exp->i += ft_strlen(var_name) + 1;
	return (var_name);
}
