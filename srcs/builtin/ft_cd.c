#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static char	*get_available_path(char *pathname, bool *print_path)
{
	char	*newpath;
	char	**cdpaths;
	char	*env_cdpath;

	if (pathname[0] == '/')
		return (canonical_path(ft_strdup(pathname)));
	if (str_equal(pathname, ".", 2) || str_equal(pathname, "./", 2)
		|| str_equal(pathname, "../", 3)
		|| str_equal(pathname, "..", 3))
		return (canonical_path(absolute_path(pathname, false)));
	env_cdpath = getenv("CDPATH");
	if (!env_cdpath)
		return (canonical_path(absolute_path(pathname, false)));
	cdpaths = ft_split(env_cdpath, ':');
	if (!cdpaths)
		return (NULL);
	if (create_path(pathname, cdpaths, &newpath) == FAILURE)
		return (NULL);
	if (!newpath)
		return (canonical_path(ft_strdup(pathname)));
	else if (print_path)
		*print_path = true;
	if (newpath[0] == '/')
		return (canonical_path(newpath));
	return (canonical_path(absolute_path(newpath, true)));
}

static int	try_absolute_path(char *input, bool *malloc_success)
{
	char	*pathname;
	bool	print_path;

	print_path = false;
	pathname = get_available_path(input, &print_path);
	if (!pathname && errno != ENOENT)
		return (FAILURE);
	if (chdir(pathname) == SYS_ERROR)
	{
		*malloc_success = true;
		if (pathname)
			free(pathname);
		return (FAILURE);
	}
	if (print_path)
		ft_putendl_fd(pathname, STDOUT_FILENO);
	return (set_working_directory(pathname));
}

static int	try_verbatim_path(char *input, bool *malloc_success)
{
	char	*pathname;

	if (chdir(input) == SYS_ERROR)
	{
		*malloc_success = true;
		return (FAILURE);
	}
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

static int	ft_cd_env(char *env)
{
	char	*pathname;

	pathname = getenv(env);
	if (!pathname)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(env, STDERR_FILENO);
		ft_putendl_fd(" not set", STDERR_FILENO);
		return (SUCCESS);
	}
	pathname = get_available_path(pathname, NULL);
	if (!pathname && errno != ENOENT)
		return (FAILURE);
	if (chdir(pathname) == SYS_ERROR)
		return (cd_error(pathname));
	if (str_equal(env, "OLDPWD", 7))
		ft_putendl_fd(pathname, STDOUT_FILENO);
	return (set_working_directory(pathname));
}

int	ft_cd(char **av)
{
	bool	malloc_success;

	malloc_success = false;
	g_sig_info.exit_status = EXIT_SUCCESS;
	if (!av[1])
		return (ft_cd_env("HOME"));
	if (str_equal(av[1], "-", 2))
		return (ft_cd_env("OLDPWD"));
	if (try_absolute_path(av[1], &malloc_success) == SUCCESS)
		return (SUCCESS);
	if (!malloc_success)
		return (FAILURE);
	if (av[1][0] != '/' && try_verbatim_path(av[1], &malloc_success) == SUCCESS)
		return (SUCCESS);
	if (!malloc_success)
		return (FAILURE);
	return (cd_error(ft_strdup(av[1])));
}
