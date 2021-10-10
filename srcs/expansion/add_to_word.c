#include "../../incs/minishell.h"

int	add_char_to_word(t_exp *exp, int pos)
{
	char	*new_word;
	size_t	word_len;

	if (!exp->exp_word)
	{
		exp->exp_word = ft_substr(exp->word, exp->i + pos - 1, 1);
		if (!exp->exp_word)
			return (FAILURE);
		exp->i += pos;
		return (SUCCESS);
	}
	word_len = ft_strlen(exp->exp_word);
	new_word = malloc(sizeof(char) * (word_len + 2));
	if (!new_word)
		return (FAILURE);
	ft_strlcpy(new_word, exp->exp_word, word_len + 1);
	ft_strlcat(new_word, &exp->word[exp->i + pos - 1], word_len + 2);
	free(exp->exp_word);
	exp->i += pos;
	exp->exp_word = new_word;
	return (SUCCESS);
}

int	add_word_in_quote_to_word(t_exp *exp)
{
	char	*addition;
	char	*tmp;
	int		j;

	j = 0;
	if (exp->in_squote)
		while (exp->word[exp->i + j] && exp->word[exp->i + j] != '\'')
			j++;
	else if (exp->in_dquote)
		while (exp->word[exp->i + j] && exp->word[exp->i + j] != '\"'
			&& exp->word[exp->i + j] != '$' && !(exp->word[exp->i + j] == '\\'
				&& ft_strchr("$\'\"\\", exp->word[exp->i + j + 1]) != NULL))
			j++;
	addition = ft_substr(exp->word, exp->i, j);
	exp->i += j;
	if (!addition)
		return (FAILURE);
	tmp = exp->exp_word;
	exp->exp_word = ft_strjoin(exp->exp_word, addition);
	if (tmp)
		free(tmp);
	free(addition);
	if (!exp->exp_word)
		return (FAILURE);
	return (SUCCESS);
}

static char	*get_var_name(t_exp *exp)
{
	int		j;
	char	*var_name;

	j = 1;
	if (isdigit(exp->word[exp->i + 1]) || exp->word[exp->i + 1] == '?')
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

int	add_var_to_word(t_exp *exp, t_env *env)
{
	char	*var_name;
	t_env	*env_var;
	char	*tmp;

	var_name = get_var_name(exp);
	if (!var_name)
		return (FAILURE);
	if (str_equal(var_name, "", 1))
	{
		free(var_name);
		return (add_char_to_word(exp, 0));
	}
	env_var = ft_find_env_var(env, var_name);
	free(var_name);
	if (!env_var)
		return (SUCCESS);
	tmp = exp->exp_word;
	exp->exp_word = ft_strjoin(exp->exp_word, env_var->value);
	if (tmp)
		free(tmp);
	if (!exp->exp_word)
		return (FAILURE);
	return (SUCCESS);
}

void	add_to_word(t_exp *exp, bool *var_exp, t_env *env)
{
	if (exp->in_squote || (exp->in_dquote && exp->word[exp->i] != '$'
			&& !(exp->word[exp->i] == '\\'
				&& ft_strchr("$\'\"\\", exp->word[exp->i + 1]))))
		exp->rlt_status = add_word_in_quote_to_word(exp);
	else if (exp->word[exp->i] == '\\'
		&& !exp->in_squote && exp->word[exp->i + 1])
		exp->rlt_status = add_char_to_word(exp, 2);
	else if (exp->word[exp->i] == '$'
		&& exp->word[exp->i + 1] && !exp->in_squote)
	{
		exp->rlt_status = add_var_to_word(exp, env);
		if (var_exp != NULL && !exp->in_dquote)
			*var_exp = true;
	}
	else
		exp->rlt_status = add_char_to_word(exp, 1);
}
