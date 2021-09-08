#include "../incs/minishell.h"

// this is absolutely wrong....

void ft_cd(int ac, char **av)
{
	if (ac != 2)
	{
		write(2, "cd: too many arguments\n", 24);
		return ;
	}
	chdir(av[1]);
	perror(NULL);
}

int main(int ac, char **av)
{
	ft_cd(ac, av);
}