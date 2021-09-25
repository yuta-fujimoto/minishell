#include "../../incs/minishell.h"

char	*get_available_path(char *pathname, bool *print_path)
{
	char	*newpath;
	char	**cdpaths;
	char	*rlt;

	if (pathname[0] == '/')
		return (ft_strdup(pathname));
	if (ft_strncmp(pathname, ".", 2) == 0 || ft_strncmp(pathname, "./", 2) == 0
		|| ft_strncmp(pathname, "../", 3) == 0
		|| ft_strncmp(pathname, "..", 3) == 0)
		return (absolute_path(pathname));
	cdpaths = ft_split(getenv("CDPATH"), ':');
	if (!cdpaths)
		return (absolute_path(pathname));
	newpath = create_path(pathname, cdpaths);
	if (!newpath)
		return (absolute_path(pathname));
	*print_path = true;
	rlt = absolute_path(newpath);
	free(newpath);
	return (rlt);
}

int	try_absolute_path(char *input)
{
	char	*pathname;
	bool	print_path;
	int		rlt;

	print_path = false;
	pathname = get_available_path(input, &print_path);
	if (!pathname)
		return (FAILURE);
	rlt = chdir(pathname);
	if (rlt != 0)
	{
		free(pathname);
		return (FAILURE);
	}
	if (print_path)
		ft_putendl_fd(pathname, STDOUT_FILENO);
	rlt = set_working_directory(pathname);
	return (rlt);
}

int	try_verbatim_path(char *input)
{
	char	*pathname;

	if (chdir(input) != 0)
		return (FAILURE);
	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		ft_putendl_fd("cd: error retrieving current \
directory: getcwd: cannot access parent \
directories: No such file or directory", STDERR_FILENO);
		pathname = getenv("PWD");
		if (pathname[ft_strlen(pathname) - 1] == '/')
			pathname = ft_strjoin(getenv("PWD"), input);
		else
			pathname = ft_strcjoin(getenv("PWD"), input, '/');
		if (!pathname)
			return (FAILURE);
	}
	return (set_working_directory(pathname));
}

int	ft_cd_env(char *env)
{
	char	*dirname;
	int		rlt;
	char	*err;

	dirname = getenv(env);
	if (!dirname)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(env, STDERR_FILENO);
		ft_putendl_fd(" not set", STDERR_FILENO);
		return (FAILURE);
	}
	rlt = chdir(dirname);
	if (rlt != 0)
	{
		err = ft_strjoin("minishell: cd ", dirname);
		perror(err);
		if (err)
			free(err);
	}
	else if (ft_strncmp(env, "OLDPWD", 7) == 0)
		ft_putendl_fd(dirname, STDOUT_FILENO);
	return (set_working_directory(ft_strdup(dirname)));
}

int	ft_cd(char **av)
{
	char	*err;

	if (!av[1])
		return (ft_cd_env("HOME"));
	if (av[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (SUCCESS);
	}
	if (ft_strncmp(av[1], "-", 2) == 0)
		return (ft_cd_env("OLDPWD"));
	if (try_absolute_path(av[1]) == SUCCESS)
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
