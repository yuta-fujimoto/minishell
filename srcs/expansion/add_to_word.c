#include "../../incs/minishell.h"

int	add_char_to_word(t_exp *exp, int pos)
{
	char	*new_str;
	size_t	word_len;

	if (!exp->exp_str)
	{
		exp->exp_str = ft_substr(&exp->str[exp->i], pos - 1, pos);
		if (!exp->exp_str)
			return (FAILURE);
		exp->i += pos;
		return (SUCCESS);
	}
	word_len = ft_strlen(exp->exp_str);
	new_str = malloc(sizeof(char) * (word_len + 2));
	if (!new_str)
		return (FAILURE);
	ft_strlcpy(new_str, exp->exp_str, word_len + 1);
	ft_strlcat(new_str, &exp->str[exp->i + pos - 1], word_len + 2);
	free(exp->exp_str);
	exp->i += pos;
	exp->exp_str = new_str;
	return (SUCCESS);
}

int	add_str_in_quote_to_word(t_exp *exp)
{
	char	*addition;
	char	*tmp;
	int		j;

	j = 0;
	if (exp->in_squote)
		while (exp->str[exp->i + j] && exp->str[exp->i + j] != '\'')
			j++;
	else if (exp->in_dquote)
		while (exp->str[exp->i + j] && exp->str[exp->i + j] != '\"' && exp->str[exp->i + j] != '$'
			&& !(exp->str[exp->i + j] == '\\' && ft_strchr("$\'\"\\", exp->str[exp->i + j + 1]) != NULL))
			j++;
	addition = ft_substr(&exp->str[exp->i], 0, j);
	exp->i += j;
	if (!addition)
		return (FAILURE);
	tmp = exp->exp_str;
	exp->exp_str = ft_strjoin(exp->exp_str, addition);
	if (tmp)
		free(tmp);
	free(addition);
	if (!exp->exp_str)
		return (FAILURE);
	return (SUCCESS);
}

static char	*get_var_name(t_exp *exp)
{
	int		j;
	char	*var_name;

	j = 1;
	if (isdigit(exp->str[exp->i]) || exp->str[exp->i] == '?')
		var_name = ft_substr(&exp->str[exp->i], 0 ,1);
	else
	{
		while (exp->str[exp->i + j] && ft_strchr("\'\"+= $", exp->str[exp->i + j]) == NULL)
			j++;
		var_name = ft_substr(&exp->str[exp->i], 0, j);
	}
	if (!var_name)
		return (NULL);
	exp->i += j;
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
	if (str_equal(var_name, "", 2))
	{
		free(var_name);
		return (add_char_to_word(exp, 1));
	}
	env_var = ft_find_env_var(env, var_name);
	free(var_name);
	if (!env_var)
		return (SUCCESS);
	tmp = exp->exp_str;
	exp->exp_str = ft_strjoin(exp->exp_str, env_var->value);
	if (tmp)
		free(tmp);
	if (!exp->exp_str)
		return (FAILURE);
	return (SUCCESS);
}
