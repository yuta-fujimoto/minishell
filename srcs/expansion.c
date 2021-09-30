#include "../incs/minishell.h"

int	update_word_with_char(char **word, char *new)
{
	char *new_word;
	size_t word_len;

	if (!*word)
	{
		*word = ft_substr(new, 0, 1);
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
	ft_strlcat(new_word, new, word_len + 2);
	free(*word);
	*word = new_word;
	return (SUCCESS);
}

char	*get_var_name(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '\'' && s[i] != '\"')
		i++;
	return (ft_substr(s, 0, i));
}

int	update_word_with_var(char **word, char *var_start, t_env *env, int *i)
{
	char	*var_name;
	char	*new_word;
	t_env	*env_var;

	var_name = get_var_name(var_start);
	printf("%s\n", var_name);
	if (!var_name)
	{
		if (*word)
			free(*word);
		return (FAILURE);
	}
	*i =  * i + ft_strlen(var_name) + 1;
	env_var = ft_find_env_var(env, var_name);
	free(var_name);
	if (!env_var)
		return (SUCCESS);
	new_word = ft_strjoin(*word, env->value);
	if (*word)
		free(*word);
	*word = new_word;
	printf("%s\n", new_word);
	fflush(NULL);
	return (SUCCESS);
}

int convert_word(t_list *lst, t_env *env)
{
	char	*word;
	int		i;
	bool	in_squote;
	bool	in_dquote;

	i = 0;
	in_squote = false;
	in_dquote = false;
	word = NULL;
	if (lst->flags != STR)
		return (SUCCESS);
	while (lst->word[i])
	{
		if (lst->word[i] == '\'' && !in_dquote && !in_squote && lst->word[i + 1] != '\0')
		{
			if (update_word_with_char(&word, &lst->word[i + 1]) == FAILURE)
				return (FAILURE);
			i += 2;
		}
		else if (lst->word[i] == '\"')
		{
			in_dquote = !in_dquote;
			i++;
		}
		else if (lst->word[i] == '\'')
		{
			in_squote = !in_squote;
			i++;
		}
		else if (lst->word[i] == '$' && lst->word[i + 1] && !in_squote)
		{
			if (update_word_with_var(&word, &lst->word[i + 1], env, &i) == FAILURE)
				return (FAILURE);
		}
		else
		{
			if (update_word_with_char(&word, &lst->word[i]) == FAILURE)
				return (FAILURE);
			i++;
		}
		
	}
	free(lst->word);
	lst->word = word;
	return (SUCCESS);
}

int	expansion(t_list *lst)
{
	t_env		*env;
	extern char		**environ;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	while (lst)
	{
		if (convert_word(lst, env) == FAILURE)
		{
			ft_envclear(&env, free);
			return (FAILURE);
		}
		lst = lst->next;
	}
	environ = list_to_environ(env);
	ft_envclear(&env, free);
	if (!environ)
		return (FAILURE);
	return (SUCCESS);
}
