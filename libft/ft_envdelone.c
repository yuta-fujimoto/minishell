#include "libft.h"

void	ft_envdelone(t_env *env, void (*del)(void*))
{
	if (env && del)
	{
		del(env->name);
		del(env->value);
		free(env);
	}
}
