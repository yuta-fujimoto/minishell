#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

char	*get_current_directory(void)
{
	char	*p;

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
