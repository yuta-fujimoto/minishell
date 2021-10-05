#include "../../incs/minishell.h"

static bool	update_in_quote(bool in_quote[2], char c, int *i)
{
	if (c == '\'' && !in_quote[1])
		in_quote[0] = !(in_quote[0]);
	else if (c == '\"' && !in_quote[0])
		in_quote[1] = !(in_quote[1]);
	else
		return (false);
	(*i)++;
	return (true);
}

static int	convert_word(t_list *lst, t_env *env)
{
	char	*new_word;
	int		i;
	bool	in_quote[2];
	int		rlt;

	i = 0;
	in_quote[0] = false;
	in_quote[1] = false;
	new_word = NULL;
	while (lst->word[i])
	{
		if (update_in_quote(in_quote, lst->word[i], &i) && !in_quote[0] && !in_quote[1])
			continue ;
		if (in_quote[0] || (in_quote[1] && lst->word[i] != '$' &&
			!(lst->word[i] == '\\' && ft_strchr("$\'\"\\", lst->word[i + 1]) != NULL)))
			rlt = add_str_in_quote_to_word(&new_word, &lst->word[i], in_quote, &i);
		else if (lst->word[i] == '\\' && !in_quote[0] && lst->word[i + 1] != '\0')
		{
			rlt = add_char_to_word(&new_word, &lst->word[i + 1]);
			i += 2;
		}
		else if (lst->word[i] == '$' && lst->word[i + 1] && !in_quote[0])
			rlt = add_var_to_word(&new_word, &lst->word[i + 1], env, &i);
		else
			rlt = add_char_to_word(&new_word, &lst->word[i++]);
		if (rlt == FAILURE)
			return (FAILURE);
	}
	free(lst->word);
	lst->word = new_word;
	return (SUCCESS);
}

int	expansion(t_list **lst)
{
	t_env	*env;
	t_list	*p;

	p = *lst;
	env = environ_to_list();
	if (!env)
		return (FAILURE);
	while (p)
	{
		if (p->flags == STR && convert_word(p, env) == FAILURE)
		{
			ft_envclear(&env, free);
			return (FAILURE);
		}
		p = p->next;
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
