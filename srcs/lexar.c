#include "../incs/minishell.h"

char	*get_str(char *line)
{
	int		cnt;
	int		quote_flg;
	char	*tmp;

	cnt = 1;
	tmp = line;
	if ((*line == '\"' || *line == '\'') && ft_strchr(line + 1, *line) == 0)
		quote_flg = *line;
	else
		quote_flg = 0;
	while (*tmp && (quote_flg != 0 || ft_strchr("><|; \t", *tmp) == NULL))
	{
		cnt++;
		if (quote_flg == 0 && (*tmp == '\"' || *tmp == '\''))
			quote_flg = *tmp;
		else if (quote_flg != 0 && (*tmp == '\"' || *tmp == '\''))
			quote_flg = 0;
		tmp++;
	}
	return (ft_substr(line, 0, cnt - 1));
}

void	lst_line_update(t_list **list, char *word, int flgs, char **line)
{
	if (**line == '\0')
		return ;
	if (word == NULL)
	{
		ft_lstclear(list, free);
		exit(1);
	}
	ft_lstadd_back(list, ft_lstnew(word, flgs));
	*line += ft_strlen(word);
}

t_list	*lexar(char *line)
{
	t_list	*list;

	list = NULL;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (str_equal(line, ">>", 2))
			lst_line_update(&list, ft_strdup(">>"), RRDIR, &line);
		else if (str_equal(line, "<<", 2))
			lst_line_update(&list, ft_strdup("<<"), LLDIR, &line);
		else if (*line == '>')
			lst_line_update(&list, ft_strdup(">"), RDIR, &line);
		else if (*line == '<')
			lst_line_update(&list, ft_strdup("<"), LDIR, &line);
		else if (*line == ';')
			lst_line_update(&list, ft_strdup(";"), SCOLON, &line);
		else if (*line == '|')
			lst_line_update(&list, ft_strdup("|"), PIPE, &line);
		else
			lst_line_update(&list, get_str(line), STR, &line);
	}
	return (list);
}
