#include "minishell.h"
#include "libft.h"

void	free_vars_vals(char **vars, char **vals)
{
	int	i;

	i = 0;
	while (vars[i])
	{
		free(vars[i]);
		if (vals[i])
			free(vals[i]);
		i++;
	}
	free(vars);
	vars = NULL;
	if (vals)
		free(vals);
	vals = NULL;
}

int	if_dollar_sign(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1]
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
				|| str[i + 1] == '?' || str[i + 1] == '{'))
			count++;
		i++;
	}
	return (count);
}

// int	calc_pos_length_of_var(char *str, int j, int *pos_after_dollar)
// {
// 	int		len;
// 	int		k;

// 	k = 0;
// 	len = 0;
// 	while (str[j] && str[j] != ' ' && str[j] != ',' && str[j] != '$'
// 			&& str[j] != '\\' && str[j] != '\'' && str[j] != '"')
// 	{
// 		if (str[j] == '{')
// 		{
// 			k = j + 1;
// 			*pos_after_dollar = k;
// 			while (str[k])
// 			{
// 				if (str[k] == '}')
// 					break;
// 				k++;
// 			}
// 		}
// 		len++;
// 		j++;
// 	}
// 	if (k != 0)
// 		len = k - *pos_after_dollar;
// 	return (len);
// }