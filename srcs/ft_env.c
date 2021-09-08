#include "../incs/minishell.h"

void	ft_env(void)
{
	char	**env;

	env = __environ;
	while (*env)
	{
		write(1, *env, ft_strlen(*env));
		write(1, "\n", 1);
		env++;
	}
}

/*int main()
{
	ft_env();
}*/
