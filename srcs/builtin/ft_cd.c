#include "../../incs/minishell.h"

char *get_available_path(char *pathname, bool *print_path)
{
	char *newpath;
	char **cdpaths;

	if (pathname[0] == '/')
		return (canonical_path(pathname));
	if (ft_strncmp(pathname, ".", 2) == 0 || ft_strncmp(pathname, "./", 2) == 0
		|| ft_strncmp(pathname, "../", 3) == 0 || ft_strncmp(pathname, "..", 3) == 0)
		return (canonical_path(absolute_path(pathname)));
	cdpaths = ft_split(getenv("CDPATH"), ':');
	if (!cdpaths)
		return (canonical_path(absolute_path(pathname)));
	newpath = create_path(pathname, cdpaths);
	if (!newpath)
		return (canonical_path(absolute_path(pathname)));
	*print_path = true;
	return (canonical_path(absolute_path(newpath)));
}

int try_canonical_path(char *input)
{
	char	*pathname;
	bool	print_path;
	int		rlt;

	print_path = false;
	pathname = get_available_path(input, &print_path);
	if (!pathname)
		return (FAILURE);
	if (!pathname)
		return (FAILURE);
	rlt = chdir(pathname);
	if (rlt != 0)
		return (FAILURE);
	if (print_path)
		ft_putendl_fd(pathname, STDOUT_FILENO);
	rlt = set_working_directory(pathname);
	return (rlt);
}

int try_verbatim_path(char *input)
{
	char *pathname;

	pathname = absolute_path(input);
	printf("%s\n", pathname);
	if (pathname)
		return (FAILURE);
	if (chdir(pathname) != 0)
		return (FAILURE);
	return (set_working_directory(pathname));
}

int ft_cd_env(char *env)
{
	char *dirname;

	dirname = getenv(env);
	if (!dirname)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(env, STDERR_FILENO);
		ft_putendl_fd(" not set", STDERR_FILENO);
		return (FAILURE);
	}
	chdir(dirname);
	if (ft_strncmp(env, "OLDPWD", 7) == 0)
		ft_putendl_fd(dirname, STDOUT_FILENO);
	return (SUCCESS);
}

int	ft_cd(char **av)
{
	char *err;

	if (!av[1])
		return (ft_cd_env("HOME"));
	if (av[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (SUCCESS);
	}
	if (ft_strncmp(av[1], "-", 2) == 0)
		return (ft_cd_env("OLDPWD"));
	if (try_canonical_path(av[1]) == SUCCESS)
		return (SUCCESS);
	if (av[1][0] != '/' && try_verbatim_path(av[1]) == SUCCESS)
		return (SUCCESS);
	err = ft_strjoin("minishell: cd: ", av[1]);
	if (!err)
		return (FAILURE);
	perror(err);
	free(err);
	return (SUCCESS);
}
