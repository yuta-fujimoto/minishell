#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "../libft/libft.h"

typedef struct s_node
{
	int		ac;
	char	**av;
	int		*str_flgs;
	int		flgs;
}	t_node;

typedef struct s_tree
{
	t_node			node;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

# define STR 0
# define RDIR 1
# define RRDIR 2
# define LDIR 3
# define LLDIR 4
# define SQUOTE 7
# define DQUOTE 8
# define PIPE 9
# define DPIPE 10 
# define AND 11
# define DAND 12
# define SCOLON 13

t_list	*lexar(char *line);
/* lexar */

t_tree	*command(t_list **lst);
int		consume(int flgs, t_list **lst);
t_tree	*parser(t_list *lst);
bool	syntax_error(t_tree *tree);
/* parser */

t_tree	*new_tree_cmd(t_node node);
t_tree	*new_tree(int flgs, t_tree *left, t_tree *right);
void	traverse_tree(t_tree *l, int h);
void	free_tree(t_tree *l);
/* tree library */

#endif