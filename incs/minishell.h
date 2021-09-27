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

typedef struct s_set
{
	t_tree	*tree;
	char	*input;
	t_list	*lst;
}	t_set;

typedef struct s_pipes
{
	int	status;
	int	fd_a[2];
	int	fd_b[2];
}				t_pipes;

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

# define FIRST_PIPE 18
# define MIDDLE_PIPE 19
# define END_PIPE 20
/* piping */

void	free_str_arr(char **str_arr);
void	free_set(t_set *set);
char	*create_path(char *cmd, char **paths);
bool	exec_cmd_error(char *cmd, char *cmd_path);
bool	free_cmd_path(char *cmd_path);
/* utils */

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

bool	wait_options(pid_t pid);
char	*create_cmd_path(t_node node);
bool	execute_input(t_tree *l, t_set *set);
/* execution */

void	ft_export_error(char *arg);
char	*get_current_directory(void);
int		identifier_type(char *s);
void	delete_env(t_env **env, char *name);
void	print_name_value(t_env *env);
void	ft_swap_env(t_env *a, t_env *b);
t_env	*environ_to_list(void);
char	**list_to_environ(t_env *env);
void	free_environ(void);
char	*get_value(char *s);
char	*get_name(char *s);
int		set_working_directory(char *pathname);
char	*absolute_path(char *pathname);
int		cd_error(char *pathname);
char	*update_environ_value(t_env *env, char *value);
char	*update_path(char **pathname, char *newcmp);
int		ft_cd(char **av);
int		ft_echo(char **av);
int		ft_export(char **av);
bool	ft_env(void);
int		ft_unset(char **av);
int		ft_pwd(void);
bool	ft_exit(char **av, t_set *set);
bool	is_buildin(char *cmd);
int		run_builtin_cmd(char **av, t_set *set);
/* builtin */

bool	execute_pipe(t_tree *parent, t_set *set);
bool	run_pipe_cmd(t_node node, t_pipes *pipes, t_set *set);
bool	pipe_exit_failure(t_pipes *pipes);
void	update_pipes_status(t_node node, t_pipes *pipes);
void	swap_fds(t_pipes *pipes);
void	close_pipes(t_pipes *pipes);
t_node	decide_cmd_node(t_tree *parent, t_pipes *pipes);
/* piping */

#endif
