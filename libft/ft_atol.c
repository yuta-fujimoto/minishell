#include "libft.h"

long	ft_atol(const char *nptr, int *flg)
{
	int		i;
	long	j;
	int		minus;

	i = 0;
	j = 0;
	minus = 1;
	*flg = 0;
	if (nptr[i] == '-')
	{
		minus *= -1;
		i++;
	}
	while ('0' <= nptr[i] && nptr[i] <= '9')
	{
		if (j > LONG_MAX / 10 || (j == LONG_MAX / 10
				&& (nptr[i] - '0') > LONG_MAX % 10)
			|| j < LONG_MIN / 10 || (j == LONG_MIN / 10
				&& (nptr[i] - '0') > (INT_MIN % 10) * (-1)))
			*flg = 1;
		j = j * 10 + (nptr[i++] - '0') * minus;
	}
	if (nptr[i] != '\0' || i == 0 || (i == 1 && nptr[0] == '-'))
		*flg = 1;
	return (j);
}
