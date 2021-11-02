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
# include <termios.h>
# include "../libft/libft.h"

typedef struct s_exp
{
	bool	in_dquote;
	bool	in_squote;
	int		rlt_status;
	char	*word;
	char	*exp_word;
	int		i;
}	t_exp;

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
	bool			exit_done;
	t_tree			*tree;
	char			*input;
	t_list			*lst;
	struct termios	t;
	unsigned int	safe_c_lflag;
	char			**safe_envrion;
	unsigned char	safe_c_vquit;	
	t_doclist		*heredoc_lst;
	t_doclist		*tmp_hdocs;
}	t_set;

typedef struct s_pipes
{
	int			status;
	int			fd_a[2];
	int			fd_b[2];
	t_pidlist	*pidlst;
}				t_pipes;

typedef struct s_redir
{
	int		status;
	int		safe_out;
	int		new_out;
	int		safe_in;
	int		new_in;
	int		r_flags;
	int		rr_flags;
	int		l_flags;
	int		permissions;
	bool	nofile;
}				t_redir;

typedef struct s_sig_info
{
	int		exit_status;
	int		signal;
	bool	heredoc;
	char	*term_stdin;
	bool	heredoc_sigint;
	bool	heredoc_sigeof;
	bool	sys_error;
	bool	child;
}				t_sig_info;

typedef struct s_pipe_info
{
	t_redir	*rdr;
	char	**cmd;
	char	*cmd_path;
	bool	touch;
	bool	path_error;
}				t_pipe_info;

# define SIGINT_CALL -2
# define SYS_ERROR -1
# define SAME 0
# define FAILURE 1
# define SUCCESS 0
# define STR 1
# define RDIR 2
# define RRDIR 3
# define LDIR 4
# define LLDIR 5
# define SQUOTE 6
# define DQUOTE 7
# define PIPE 8
# define DPIPE 9 
# define AND 10
# define DAND 11
# define SCOLON 12
# define STR_VAL 13

/* buildin_export */
# define ADD 14
# define UPDATE 15
# define ERROR 16
# define NOTHING 17

/* piping */
# define FIRST_PIPE 18
# define MIDDLE_PIPE 19
# define END_PIPE 20

/* CUSTOM EXIT STATUSES */
# define CHILD_FAILURE 21

/* TERMIOS FLAGS INCLUDING ECHOCTL */
# define C_LFLAGS 536872335

void	free_str_arr(char **str_arr);
void	free_set(t_set *set);
int		create_path(char *cmd, char **paths, char **cmd_path);
int		exec_cmd_error(char *cmd, char *cmd_path, bool malloc_failure);
bool	str_equal(char *s1, char *s2, size_t n);
void	print_str(unsigned int i, char *s);
bool	mod_termios_attr(t_set *set, int init);
void	ms_exit(t_set *set, int exit_status, bool exit_done);
bool	set_sys_error(void);
bool	is_acceptable_error(int errnum);
/* utils */

void	init_env(t_set *set);
void	ms_init(t_set *set);
/* init */

void	lexar(t_set *set);
/* lexar */

bool	is_word_in_dquote(t_exp *exp);
bool	is_valid(t_exp *exp);
int		add_char_to_word(t_exp *exp, int pos);
int		add_str_in_quote_to_word(t_exp *exp);
int		add_var_to_word(t_exp *exp, t_env *env);
void	add_to_word(t_exp *exp, bool *var_exp, t_env *env);
char	*get_var_name(t_exp *exp);
int		eliminate_null_node(t_node *exp_node, t_node *node);
int		split_argv_by_blank(t_node *node);
int		init_exp_node(t_node **exp_node, t_node *node);
t_node	*finish_expansion_node(t_env **env, t_node *exp_node, int ret);
int		copy_node(t_node *exp_node, t_node *node, int i);
t_node	*expansion_node(t_node *node);
int		expansion(char **exp_word, char **word, t_env *env,
			bool *var_expansion);
t_node	*expansion_conclude(t_env **env, char *free_s, t_node *exp_node);
int		expansion_node_conclude(t_node *node, int rlt);
/* expansion */

t_tree	*command(t_list **lst, bool *malloc_err);
int		consume(int flgs, t_list **lst);
bool	parser(t_set *set);
bool	syntax_error(t_tree *tree);
void	parser_error(t_set *set);
/* parser */

t_tree	*new_tree_cmd(t_node node, bool *malloc_err);
t_tree	*new_tree(int flgs, t_tree *left, t_tree *right, bool *malloc_err);
void	free_tree(t_tree *l);
/* tree library */

bool	wait_options(pid_t pid, bool pipeline);
int		create_cmd_path(char **cmd, char **cmd_path, bool *path_error);
bool	execute_input(t_tree *l, t_set *set, int *rlt);
bool	execute_simple_cmd(t_node node, t_set *set, t_redir *redir);
bool	minishell_error(t_redir *redir, int *rlt, bool no_prnt);
bool	free_cmd_path(char *cmd_path);
int		command_not_found(char *cmd, bool path_error);
/* execution */

void	ft_export_error(char *arg);
char	*get_current_directory(void);
int		identifier_type(char *s);
void	delete_env(t_env **env, char *name);
void	print_name_value(t_env *env);
void	ft_swap_env(t_env *a, t_env *b);
t_env	*environ_to_list(void);
int		list_to_environ(t_env *env);
void	free_environ(void);
char	*get_value(char *s);
char	*get_name(char *s);
int		set_working_directory(char *pathname);
char	*absolute_path(char *pathname, bool free_arg);
int		cd_error(char *pathname);
char	*canonical_path(char *pathname);
char	*update_environ_value(t_env *env, char *value);
int		ft_cd(char **av);
int		ft_echo(char **av);
int		ft_export(char **av);
bool	ft_env(void);
int		ft_unset(char **av);
int		ft_pwd(void);
bool	ft_exit(char **av, t_set *set, bool print_exit);
bool	is_buildin(char *cmd);
int		run_builtin_cmd(char **av, t_set *set, bool print_exit);
/* builtin */

bool	execute_pipeline(t_tree *parent, t_set *set, t_redir *redir);
bool	run_pipe_cmd(t_node node, t_pipes *pipes, t_set *set, t_redir *redir);
bool	pipe_exit_failure(t_pipes *pipes);
void	update_pipes_status(t_node node, t_pipes *pipes);
void	swap_fds(t_pipes *pipes);
bool	close_pipes(t_pipes *pipes);
t_node	decide_cmd_node(t_tree *parent, t_pipes *pipes);
void	run_child(t_node *n, t_pipes *pipes, t_set *set, t_pipe_info *p_info);
/* piping */

bool	close_fd(int fd, int rlt);
bool	reset_stdio_fd(t_redir *redir, int rlt);
bool	ms_redirection(t_node *node, t_redir *redir, t_doclist **hdocs);
bool	is_rdir(int str_flg);
bool	is_open_fd(int fd);
bool	end_redirection(char **cmd, t_redir *redir, int rlt);
bool	has_redirection(t_node *node);
bool	set_redirection(char **cmd, int i, t_redir *redir, t_doclist *hdocs);
int		handle_heredoc(int fds[2], char *delimiter);
char	**get_cmd(t_node *node, t_set *set, t_redir *redir, bool *touch);
char	**create_new_cmd(t_node *node, bool *touch);
bool	redirect_fds(t_redir *redir);
/* redirection */

bool	init_heredocs(t_tree *parent, t_set *set, int *rlt);
bool	has_heredoc(char **av);
void	close_heredocs(t_doclist *hdocs);
void	update_heredocs(t_node exp_node, t_set *set);
/* heredocs */

#endif
