#include "minishell.h"
#include "libft.h"

void	free_vars_vals(char **vars, char **vals)
{
	int i;

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

//  && str[i + 2] == '?' && str[i + 3] == '}')))
int if_dollar_sign(char *str)
{
	int i;
	int count;
		
	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1]
		&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'
			|| str[i + 1] == '{'))
			count++;
		i++;
	}
	return (count);
}

int	is_varname_format_brackets(char *str, int i)
{
	i++;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != '}')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '}')
		return (i);
	else
		return (0);
}

int	is_varname_format(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '{')
		return (is_varname_format_brackets(str, i) - 1);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != ' ' && str[i] != ',' && str[i] != '$'
			&& str[i] != '\\' && str[i] != '\'' && str[i] != '"')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (i);
		i++;
	}
	return (i);
}

char	*fill_words_with_real_vals(char *str, char **vars, char **vals, int t_len)
{
	int		i;
	int		j;
	int		k;
	int		o;
	char	*word;

	word = malloc(sizeof(char) * (t_len + 1));
	if (!word)
		return (NULL);
	i = 0;
	k = 0;
	j = 0;
	while(j < t_len)
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != '$')
		{
			i++;
			if (str[i] == '{')
				i++;
			if (vars[k])
			{
				o = 0;
				while(vals[k][o])
					word[j++] = vals[k][o++];
				i += ft_strlen(vars[k]);
				if (str[i] == '}')
					i++;
				k++;
			}
			else
				word[j++] = '$';
		}
		else
			word[j++] = str[i++];
	}
	word[j] = '\0';
	return (word);
}

char	*replace_init_val_by_real_val(t_varlist **head_var, char ***vars, char ***vals, char *str)
{
	char	*word;
	int		t_len;

	t_len = get_vals_and_tot_len(str, *vals, *vars, head_var);
	word = fill_words_with_real_vals(str, *vars, *vals, t_len);
	if (!word)
	{
		free_vars_vals(*vars, *vals);
		return (NULL);
	}
	return (word);
}