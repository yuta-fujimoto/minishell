#include "../incs/minishell.h"

int	update_word_with_char(char **word, char *new)
{
	char	*new_word;
	size_t	word_len;

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
	if (isdigit(s[0]) || s[0] == '?')
		return (ft_substr(s, 0 ,1));
	while (s[i] && s[i] != '\'' && s[i] != '\"' && s[i] != '+' && s[i] != '=' && s[i] != ' ')
		i++;
	return (ft_substr(s, 0, i));
}

int	update_word_with_var(char **word, char *var_start, t_env *env, int *i)
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
		return (update_word_with_char(word, "$"));
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
	if (str_equal(lst->word, "\"\"", 3) || str_equal(lst->word, "\'\'", 3))
	{
		word = lst->word;
		lst->word = ft_strdup("");
		free(word);
		if (!lst->word)
			return (FAILURE);
		return (SUCCESS);
	}
	while (lst->word[i])
	{
		if (lst->word[i] == '\\' && !in_squote && lst->word[i + 1] != '\0')
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

void eliminate_null_node(t_list **lst)
{
	t_list *tmp;
	t_list *p;
	t_list *prev;

	p = *lst;
	prev = NULL;
	while (p)
	{
		if (!p->word && prev)
		{
			tmp = p;
			prev->next = p->next;
			p = p->next;
			ft_lstdelone(tmp, free);
		}
		else if (!p->word)
		{
			tmp = *lst;
			*lst = (*lst)->next;
			p = p->next;
			ft_lstdelone(tmp, free);
		}
		else
		{
			prev = p;
			p = p->next;
		}
	}
}

int	expansion(t_list **lst)
{
	t_env	*env;
	t_list	*tmp;

	tmp = *lst;
	env = environ_to_list();
	if (!env)
		return (FAILURE);
	while (tmp)
	{
		if (convert_word(tmp, env) == FAILURE)
		{
			ft_envclear(&env, free);
			return (FAILURE);
		}
		tmp = tmp->next;
	}
	eliminate_null_node(lst);
	if (list_to_environ(env) == FAILURE)
	{
		ft_envclear(&env, free);
		return (FAILURE);
	}
	ft_envclear(&env, free);
	return (SUCCESS);
}
