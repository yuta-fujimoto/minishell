#include "libft.h"

t_env	*ft_envnew(char *name, char *value)
{
	t_env	*new;

	if (!name || !value)
	{
		if (name)
			free(name);
		if (value)
			free(value);
		return (NULL);
	}
	new = malloc(sizeof(t_env));
	if (!new)
	{
		free(name);
		free(value);
		return (NULL);
	}
	new->name = name;
	new->value = value;
	new->next = NULL;
	return (new);
}
