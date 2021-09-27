#include "../incs/minishell.h"

char *update_word(char *word, char *new, size_t new_len)
{
	char *new_word;
	size_t i;
	size_t j;

	i = -1;
	j = -1;
	if (!word)
		return (ft_substr(new, 0, new_len)));
	new_word = malloc(sizeof(char) * (ft_strlen(word) + new_len + 1);
	if (!new_word)
		return (NULL);
	while (word[++i])
		new_word[i] = word[i];
	while ((size_t)++j < new_len)
		new_word[i + j] = new[j];
	free(word);
	return (new_word);
}

char *convert_word(t_list *lst, t_list *prev, t_env *env, bool *str_flg)
{
	char *result;
	int i;
	bool in_squote;
	bool in_dquote;

	i = 0;
	in_squote = false;
	result = NULL;
	while (lst->word[i])
	{
		if (lst->word[i] == '\'' && !in_dquote && !in_squote && lst->word[i + 1] != '\0')
		{
			result = update_word(result, &lst->word[i + 1], 1);
			if (!result)
				return (NULL);
			i++;
		}
		if (lst->word[i] == '$' && !in_squote)
		{

		}

	}
}

int expansion(t_list *lst)
{
	t_list		*prev;
	t_env		*env;
	extern char	**environ;
	bool		str_flg;

	env = environ_to_list();
	if (!env)
		return (FAILURE);
	prev = NULL;
	str_flg = true;
	while (lst)
	{
		lst->word = convert_word(lst, prev, env, &str_flg);
		prev = lst;
		lst = lst->next;
	}
	environ = list_to_environ(env);
	if (!environ);
		return (FAILURE);
	return (SUCCESS);
}
