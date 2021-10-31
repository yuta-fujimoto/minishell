/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 22:39:12 by fyuta             #+#    #+#             */
/*   Updated: 2021/10/27 20:24:50 by jthompso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_list
{
	char			*word;
	int				flags;
	struct s_list	*next;
}				t_list;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pidlist
{
	pid_t				pid;
	struct s_pidlist	*next;
}				t_pidlist;

typedef struct s_doclist
{
	int					fds[2];
	char				*delim;
	struct s_doclist	*next;
}				t_doclist;

int				ft_atoi(const char *nptr);
long			ft_atol(const char *nptr, int *flg);
void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
char			*ft_itoa(int n);
void			*ft_memccpy(void *dest, const void *src, int c, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			*ft_memset(void *s, int c, size_t n);
void			ft_putchar_fd(char c, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
void			ft_putstr_fd(char *s, int fd);
char			**ft_split(char const *str, char c);
char			*ft_strchr(const char *s, int c);
char			*ft_strdup(const char *src);
void			ft_striteri(char **s, void (*f)(unsigned int, char *));
char			*ft_strjoin(char const *s1, char const *s2);
size_t			ft_strlcat(char *dest, char *src, size_t size);
size_t			ft_strlcpy(char *dest, char *src, size_t size);
size_t			ft_strlen(const char *s);
char			*ft_strmapi(char const *s, char (*f)(unsigned	int, char));
int				ft_strncmp(char *s1, char *s2, size_t n);
char			*ft_strnstr(char *str, char *to_find, size_t len);
char			*ft_strrchr(const char *s, int c);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_substr(char const *s, unsigned int start, size_t len);
int				ft_tolower(int c);
int				ft_toupper(int c);
bool			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstadd_front(t_list **lst, t_list *new);
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));
t_list			*ft_lstnew(void *word, int flgs);
int				ft_lstsize(t_list *lst);
char			*ft_strcjoin(char const *s1, char const *s2, int c);
bool			ft_envadd_back(t_env **env, t_env *new);
void			ft_envclear(t_env **env, void (*del)(void *));
void			ft_envdelone(t_env *env, void (*del)(void*));
void			ft_enviter(t_env *env, void (*f)(void *));
t_env			*ft_envnew(char *name, char *value);
int				ft_envsize(t_env *env);
t_env			*ft_find_env_var(t_env *env, char *name);
int				ft_str_arr_len(char **str_arr);
int				ft_str_arr_search(char **str_arr, char *search);
void			ft_free_str_arr(char **str_arr);
t_pidlist		*ft_pidlstnew(pid_t pid);
int				ft_pidlstsize(t_pidlist *pidlst);
t_pidlist		*ft_pidlstlast(t_pidlist *pidlst);
void			ft_pidlstclear(t_pidlist **pidlst);
bool			ft_pidlstadd_back(t_pidlist **pidlst, t_pidlist *new);
t_doclist		*ft_doclstnew(int fds[2], char *delim);
int				ft_doclstsize(t_doclist *doclst);
t_doclist		*ft_doclstlast(t_doclist *doclst);
void			ft_doclstclear(t_doclist **doclst);
bool			ft_doclstadd_back(t_doclist **doclst, t_doclist *new);

#endif
