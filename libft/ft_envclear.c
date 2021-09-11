#include "libft.h"

void	ft_envclear(t_env **env, void (*del)(void *))
{
	t_env	*tmp;

	if (!env)
		return ;
	while (*env)
	{
		if (del)
		{
			del((*env)->name);
			del((*env)->value);
		}
		tmp = *env;
		*env = (*env)->next;
		free(tmp);
	}
	*env = NULL;
}
