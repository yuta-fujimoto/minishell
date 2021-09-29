#include "../incs/minishell.h"

void ft_echo(char **av)
{
	int nflag;

	nflag = 0;
	while (*(++av) && str_equal(*av, "-n", 3))
		nflag = 1;
	while (*av)
	{
		write(1, *av, ft_strlen(*av));
		if (*(++av))
			write(1, " ", 1);
	}
	if (!nflag)
		write(1, "\n", 1);
	return;
}

/*int main(int ac, char **av)
{
	ft_echo(av);
}
*/
