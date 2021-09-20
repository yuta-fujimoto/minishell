#include "../../incs/minishell.h"

int	set_working_directory(char *newpath)
{
	char	*cmd[3];

	cmd[0] = "export";
	cmd[1] = ft_strjoin("PWD=", newpath);
	cmd[2] = NULL;
	if (!cmd[0])
		return (FAILURE);
	if (ft_export(cmd) == FAILURE)
		return (FAILURE);
	free(cmd[1]);
	return (SUCCESS);
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

char *get_path(char *pathname)
{
	char *newpath;
	char **cdpaths;

	if (pathname[0] == '/')
		return (ft_strdup(pathname));
	if (ft_strncmp(pathname, ".", 2) == 0 || ft_strncmp(pathname, "./", 2) == 0
		|| ft_strncmp(pathname, "../", 3) == 0 || ft_strncmp(pathname, "..", 3) == 0)
		return (ft_strdup(pathname));
	cdpaths = ft_split(getenv("CDPATH"), ':');
	if (!cdpaths)
		return (ft_strdup(pathname));
	newpath = create_path(pathname, cdpaths);
	if (!newpath)
		return (ft_strdup(pathname));
	return (newpath);
}

int	ft_cd(char **av)
{
	int	rlt;
	char *pathname;

	if (!av[1])
		return (ft_cd_env("HOME"));
	if (av[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (SUCCESS);
	}
	if (ft_strncmp(av[1], "-", 2) == 0)
		return (ft_cd_env("OLDPWD"));
	pathname = absolute_path(pathname);
	if (!pathname)
		return (FAILURE);
	rlt = chdir(pathname);
	if (rlt != 0)
		perror("minishell: cd ");
	else if (set_working_directory(av[1]) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
