#include "../../incs/minishell.h"

static char	*logical_getcwd(void)
{
	struct stat	st1;
	struct stat	st2;
	char		*wd;
	char		*p;

	wd = getenv("PWD");
	if (!wd || wd[0] != '/')
		return (NULL);
	p = ft_strnstr(wd, "/.", 2);
	while (p)
	{
		if (!p[2] || p[2] == '/' || (p[2] == '.' && (!p[3] || p[3] == '/')))
			return (NULL);
		p = ft_strnstr((++p), "/.", 2);
	}
	if (stat(wd, &st1) == 0 && stat(".", &st2) == 0 && st1.st_ino == st2.st_ino)
		return (wd);
	return (NULL);
}

char	*get_current_directory(void)
{
	char	*p;

	p = logical_getcwd();
	if (p)
		return (ft_strdup(p));
	p = getcwd(NULL, 0);
	if (p)
		return (p);
	p = getenv("PWD");
	if (p)
		return (ft_strdup(p));
	errno = ENOENT;
	return (NULL);
}

int	ft_pwd(void)
{
	char	*pathname;

	g_sig_info.exit_status = EXIT_SUCCESS;
	pathname = get_current_directory();
	if (pathname)
	{
		ft_putendl_fd(pathname, STDOUT_FILENO);
		free(pathname);
		return (SUCCESS);
	}
	g_sig_info.exit_status = EXIT_FAILURE;
	if (errno == ENOENT)
	{
		perror("minishell: pwd");
		return (SUCCESS);
	}
	return (FAILURE);
}
