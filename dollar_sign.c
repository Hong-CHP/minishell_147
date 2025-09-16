#include "minishell.h"
#include "libft.h"

int	calc_pos_length_of_var(char *str, int j, int *pos_after_dollar)
{
	int		len;
	int		k;

	k = 0;
	len = 0;
	while (str[j] && str[j] != ' ' && str[j] != ',' && str[j] != '$'
			&& str[j] != '\\' && str[j] != '\'' && str[j] != '"')
	{
		if (str[j] == '{')
		{
			k = j + 1;
			*pos_after_dollar = k;
			while (str[k])
			{
				if (str[k] == '}')
					break;
				k++;
			}
		}
		len++;
		j++;
	}
	if (k != 0)
		len = k - *pos_after_dollar;
	return (len);
}

char	*fill_multi_vars(char *str, int i)
{
	int		pos_after_dollar;
	int		j;
	char	*var;
	// int		len;
	int		var_len;

	j = i + 1;
	if (*(str + j) == '?' || ft_strnstr(str + j, "{?}", ft_strlen(str)))
		return (ft_strdup("?"));
	var_len = is_varname_format(str + j);
	if (!var_len)
		return (NULL);
	// printf("var len is %d\n", var_len);
	if (str[j] == '{')
		pos_after_dollar = j + 1;
	else
		pos_after_dollar = j;
	// len = calc_pos_length_of_var(str, j, &pos_after_dollar);
	var = ft_substr(str, pos_after_dollar, var_len);
	return (var);
}

char	**find_dollar_sign(char *str, int nb_var)
{
	int		i;
	int		k;
	char	**vars;

	vars = malloc(sizeof(char *) * (nb_var + 1));
	if (!vars)
		return (NULL);
	i = 0;
	k = 0;
	while(str[i])
	{
		if (str[i] == '$' && str[i + 1])
		//  && 
		// 	(ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'
		// 	|| (str[i + 1] == '{' && str[i + 2] == '?' && str[i + 3] == '}')))
		{
			if (str[i + 1] >= '0' && str[i + 1] <= '9')
				vars[k] = ft_substr(str, i + 1, 1);
			else
			{
				vars[k] = fill_multi_vars(str, i);
				if (!vars[k])
					free_vars_vals(vars, NULL);
			}
			// if (vars[k])
			// 	printf("vars[k] is %s\n", vars[k]);
			k++;
		}
		i++;
	}
	vars[k] = NULL;
	return (vars);
}

char *check_same_var_in_varlist(char *var, t_varlist **head_var)
{
    t_varlist *cur;
    char      *tmp;
    
    if (!var || !head_var || !*head_var)
        return NULL;
    tmp = NULL;
    cur = *head_var;
    while (cur)
    {
        if (ft_strcmp(var, cur->var_data->var) == 0)
        {
            tmp = ft_strdup(cur->var_data->val);
            return (tmp);
        }
        cur = cur->next;
    }
    return (NULL);
}

int    get_vals_and_tot_len(char *str, char **vals, char **vars, t_varlist **head_var)
{
    int i;
    size_t vars_len;
    size_t vals_len;
    int t_len;

    i = 0;
    vars_len = 0;
    vals_len = 0;
    while (vars[i])
    {
		vars_len += ft_strlen(vars[i]);
        vals[i] = check_same_var_in_varlist(vars[i], head_var);
		if (!vals[i] && ft_strcmp(vars[i], "?") == 0)
			vals[i] = ft_itoa(g_exit_status);
		else if (!vals[i])
			vals[i] = ft_strdup("");
        vals_len += ft_strlen(vals[i]);
        i++;
    }
    vals[i] = NULL;
	t_len = ft_strlen(str) - if_dollar_sign(str) - vars_len + vals_len;
    return (t_len);
}

void    fill_words_with_real_vals(char *word, char *str, char **vars, char **vals, int t_len)
{
    int i;
    int j;
    int k;
    int o;
    
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
}

char	*reg_dollar_sign(char *str, t_varlist **head_var)
{
	int		nb_vars;
	char	**vars;
	char	*word;
	char	**vals;
	int		t_len;

	word = NULL;
	nb_vars = if_dollar_sign(str);
	if (nb_vars == 0)
		return (ft_strdup(str));
	vars = find_dollar_sign(str, nb_vars);
	vals = malloc(sizeof(char *) * (nb_vars + 1));
	if (!vals)
	{
		free_vars_vals(vars, vals);
		return (NULL);
	}
	t_len = get_vals_and_tot_len(str, vals, vars, head_var);
	word = malloc(sizeof(char) * (t_len + 1));
	if (!word)
	{
		free_vars_vals(vars, vals);
		return (NULL);
	}
	fill_words_with_real_vals(word, str, vars, vals, t_len);
	free_vars_vals(vars, vals);
	return (word);
}
