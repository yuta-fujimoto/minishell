#include "../../incs/minishell.h"

int	ft_pwd(void)
{
	char	pathname[PATH_MAX];

	if (getcwd(pathname, PATH_MAX))
	{
		ft_putendl_fd(pathname, STDOUT_FILENO);
		return (SUCCESS);
	}
	errno = ENAMETOOLONG;
	return (FAILURE);
}
