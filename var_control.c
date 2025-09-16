#include "minishell.h"
#include "libft.h"

char	**if_val_after_equal(char *input, int i, char **val)
{
	int		count_val;

	count_val = 0;
	while (input[i])
	{
		count_val++;
		i++;
	}
	if (count_val == 0 || input[i] == ' ')
		return (NULL);
	*(val) = malloc(sizeof(char) * (count_val + 1));
	if (!*(val))
		return (NULL);
	ft_memset(*(val), 0, count_val + 1);
	return (val);
}

char	*if_variable_val(char *input)
{
	char	*val;

	int		i;

	i = 0;
	while (input[i] && input[i] != '=')
		i++;
	if (input[i] == '=')
		i++;
	else
	{
		val = ft_strdup("");
		return (val);
	}
	if (!input[i])
	{
		val = ft_strdup("");
		return (val);
	}
	if (!if_val_after_equal(input, i, &val))
		return (NULL);
	return (val);
}

int	is_valide_varname(char *input)
{
	int i;
	
	if (!input || !input[0])
		return (0);
	if (!ft_isalpha(input[0]) && input[0] != '_' && input[0] != '$')
		return (0);
	i = 1;
	while (input[i] && input[i] != '=')
	{
		if (!ft_isalnum(input[i]) && input[i] != '_' && input[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

char	*if_variable_var(char *input)
{
	char	*var;
	int		name_len;

	if (!is_valide_varname(input))
		return (NULL);
	name_len = 0;
	while (input[name_len] && input[name_len] != '=')
		name_len++;
	var = malloc(sizeof(char) * (name_len + 1));
	if (!var)
		return (NULL);
	ft_memset(var, 0, name_len + 1);
	return (var);
}