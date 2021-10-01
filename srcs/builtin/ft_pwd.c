#include "../../incs/minishell.h"

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

	pathname = get_current_directory();
	if (pathname)
	{
		ft_putendl_fd(pathname, STDOUT_FILENO);
		free(pathname);
		return (SUCCESS);
	}
	if (errno == ENOENT)
	{
		perror("minishell: pwd");
		return (SUCCESS);
	}
	return (FAILURE);
}
