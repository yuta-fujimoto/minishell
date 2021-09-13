#include "libft.h"

void	ft_enviter(t_env *env, void (*f)(void *))
{
	if (!f)
		return ;
	while (env)
	{
		f(env->name);
		f(env->value);
		env = env->next;
	}
}
