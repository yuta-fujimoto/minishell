#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <limits.h>
# include <errno.h>
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

# define FAILURE 1
# define SUCCESS 0
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

# define ADD 14
# define UPDATE 15
# define ERROR 16
# define NOTHING 17
/* buildin_export */

# define PATH_MAX 1024

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

bool	execute_input(t_tree *l);
/* execution */

int		identifier_type(char *s);
void	delete_env(t_env **env, char *name);
void	print_name_value(t_env *env);
void	ft_swap_env(t_env *a, t_env *b);
char	**init_environ(char **environ);
t_env	*environ_to_list(char **environ);
char	**list_to_environ(t_env *env);
void	free_environ(char **environ);
char	*get_value(char *s);
char	*get_name(char *s);
int		ft_export(char **av, t_env **env);
bool	ft_env(void);
int		ft_unset(char **av, t_env **env);
int		ft_pwd(void);
bool	is_buildin(char *cmd);
int		run_builtin_cmd(char **av);
/* builtin */

#endif
