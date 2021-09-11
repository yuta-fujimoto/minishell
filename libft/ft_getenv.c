#include "libft.h"

t_env	*ft_getenv(t_env *env, char *name)
{
	if (!env || !name)
		return (NULL);
	while (env)
	{
		if (ft_strncmp(env->name, name, UINT_MAX) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
