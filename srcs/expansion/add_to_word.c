#include "../../incs/minishell.h"

int	add_char_to_word(char **word, char *lst_word)
{
	char	*new_word;
	size_t	word_len;

	if (!*word)
	{
		*word = ft_substr(lst_word, 0, 1);
		if (!*word)
			return (FAILURE);
		return (SUCCESS);
	}
	word_len = ft_strlen(*word);
	new_word = malloc(sizeof(char) * (word_len + 2));
	if (!new_word)
		return (FAILURE);
	ft_strlcpy(new_word, *word, word_len + 1);
	ft_strlcat(new_word, lst_word, word_len + 2);
	free(*word);
	*word = new_word;
	return (SUCCESS);
}

int	add_str_in_quote_to_word(char **word, char *lst_word, bool in_quote[2], int *i)
{
	char	*addition;
	char	*tmp;
	int		j;

	j = 0;
	if (in_quote[0])
		while (lst_word[j] && lst_word[j] != '\'')
			j++;
	else if (in_quote[1])
		while (lst_word[j] && lst_word[j] != '\"' && lst_word[j] != '$'
			&& !(lst_word[j] == '\\' && ft_strchr("$\'\"\\", lst_word[j + 1]) != NULL))
			j++;
	*i += j;
	addition = ft_substr(lst_word, 0, j);
	if (!addition)
		return (FAILURE);
	tmp = *word;
	*word = ft_strjoin(*word, addition);
	if (tmp)
		free(tmp);
	free(addition);
	if (!*word)
		return (FAILURE);
	return (SUCCESS);
}

static char	*get_var_name(char *s, int *i)
{
	int		j;
	char	*var_name;

	j = 0;
	if (isdigit(s[0]) || s[0] == '?')
		var_name = ft_substr(s, 0 ,1);
	else
	{
		while (s[j] && ft_strchr("\'\"+= ", s[j]) == NULL)
			j++;
		var_name = ft_substr(s, 0, j);
	}
	if (!var_name)
		return (NULL);
	*i += ft_strlen(var_name) + 1;
	return (var_name);
}

int	add_var_to_word(char **word, char *var_start, t_env *env, int *i)
{
	char	*var_name;
	t_env	*env_var;
	char	*tmp;

	var_name = get_var_name(var_start, i);
	if (!var_name)
		return (FAILURE);
	if (str_equal(var_name, "", 2))
	{
		free(var_name);
		return (add_char_to_word(word, "$"));
	}
	env_var = ft_find_env_var(env, var_name);
	free(var_name);
	if (!env_var)
		return (SUCCESS);
	tmp = *word;
	*word = ft_strjoin(*word, env_var->value);
	if (tmp)
		free(tmp);
	if (!*word)
		return (FAILURE);
	return (SUCCESS);
}
