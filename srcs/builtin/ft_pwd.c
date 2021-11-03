#include "../../incs/minishell.h"
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

extern t_sig_info	g_sig_info;

static char	*logical_getcwd(void)
{
	struct stat	lg;
	struct stat	phy;
	char		*wd;

	wd = getenv("PWD");
	if (!wd || wd[0] != '/')
		return (NULL);
	if (stat(wd, &lg) == 0 && stat(".", &phy) == 0
		&& lg.st_ino == phy.st_ino && lg.st_dev == phy.st_dev)
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
