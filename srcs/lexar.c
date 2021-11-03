#include "../incs/minishell.h"
#include <unistd.h>

static void	lexar_error(t_set *set, int rlt)
{
	if (rlt == SUCCESS)
		return ;
	ft_putendl_fd("minishell:error", STDERR_FILENO);
	ms_exit(set, EXIT_FAILURE, true);
}

static char	*get_str(char *line)
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
		else if (quote_flg != 0 && *tmp == quote_flg)
			quote_flg = 0;
		else if (*tmp == '\\' && *(tmp + 1) != '\0')
		{
			tmp++;
			cnt++;
		}
		tmp++;
	}
	return (ft_substr(line, 0, cnt - 1));
}

static int	lst_line_update(t_set *set, char *word, int flgs, char **line)
{
	if (word == NULL)
		lexar_error(set, FAILURE);
	if (**line == '\0')
	{
		free(word);
		return (SUCCESS);
	}
	if (!ft_lstadd_back(&set->lst, ft_lstnew(word, flgs)))
		return (FAILURE);
	*line += ft_strlen(word);
	return (SUCCESS);
}

void	lexar(t_set *set)
{
	char	*line;
	int		rlt;

	line = set->input;
	set->lst = NULL;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (str_equal(line, ">>", 2))
			rlt = lst_line_update(set, ft_strdup(">>"), RRDIR, &line);
		else if (str_equal(line, "<<", 2))
			rlt = lst_line_update(set, ft_strdup("<<"), LLDIR, &line);
		else if (*line == '>')
			rlt = lst_line_update(set, ft_strdup(">"), RDIR, &line);
		else if (*line == '<')
			rlt = lst_line_update(set, ft_strdup("<"), LDIR, &line);
		else if (*line == ';')
			rlt = lst_line_update(set, ft_strdup(";"), SCOLON, &line);
		else if (*line == '|')
			rlt = lst_line_update(set, ft_strdup("|"), PIPE, &line);
		else
			rlt = lst_line_update(set, get_str(line), STR, &line);
		lexar_error(set, rlt);
	}
}
