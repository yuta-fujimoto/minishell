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
	{
		free(*word);
		return (FAILURE);
	}
	ft_strlcpy(new_word, *word, word_len + 1);
	ft_strlcat(new_word, lst_word, word_len + 2);
	free(*word);
	*word = new_word;
	return (SUCCESS);
}

int	add_str_in_quote_to_word(char **word, char *lst_word, bool in_quote[2], int *i)
{
	char	*new_word;
	char	*tmp;
	int		j;

	j = 0;
	if (in_quote[0])
	{
		while (lst_word[j] && lst_word[j] != '\'')
			j++;
	}
	else if (in_quote[1])
	{
		while (lst_word[j] && lst_word[j] != '\"' && lst_word[j] != '$' && !(lst_word[j] == '\\' && ft_strchr("$\'\"\\", lst_word[j + 1]) != NULL))
			j++;
	}
	*i += j;
	new_word = ft_substr(lst_word, 0, j);
	if (!new_word)
		return (FAILURE);
	if (!*word)
	{
		*word = new_word;
		return (SUCCESS);
	}
	tmp = *word;
	*word = ft_strjoin(*word, new_word);
	free(tmp);
	free(new_word);
	if (!*word)
		return (FAILURE);
	return (SUCCESS);
}

static char	*get_var_name(char *s)
{
	int	i;

	i = 0;
	if (isdigit(s[0]) || s[0] == '?')
		return (ft_substr(s, 0 ,1));
	while (s[i] && s[i] != '\'' && s[i] != '\"' && s[i] != '+' && s[i] != '=' && s[i] != ' ')
		i++;
	return (ft_substr(s, 0, i));
}

int	add_var_to_word(char **word, char *var_start, t_env *env, int *i)
{
	char	*var_name;
	char	*new_word;
	t_env	*env_var;

	var_name = get_var_name(var_start);
	if (!var_name)
	{
		if (*word)
			free(*word);
		return (FAILURE);
	}
	if (str_equal(var_name, "", 2))
	{
		(*i)++;
		free(var_name);
		return (add_char_to_word(word, "$"));
	}
	*i = *i + ft_strlen(var_name) + 1;
	env_var = ft_find_env_var(env, var_name);
	free(var_name);
	if (!env_var)
		return (SUCCESS);
	if (!*word)
		new_word = ft_strdup(env->value);
	else
		new_word = ft_strjoin(*word, env->value);
	if (*word)
		free(*word);
	if (!new_word)
		return (FAILURE);
	*word = new_word;
	return (SUCCESS);
}
