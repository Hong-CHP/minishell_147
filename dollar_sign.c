#include "minishell.h"
#include "libft.h"

char	*fill_multi_vars(char *str, int i)
{
	int		pos_after_dollar;
	int		j;
	char	*var;
	int		var_len;

	j = i + 1;
	if (*(str + j) == '?' || ft_strnstr(str + j, "{?}", ft_strlen(str)))
		return (ft_strdup("?"));
	var_len = is_varname_format(str + j);
	if (!var_len)
		return (NULL);
	if (str[j] == '{')
		pos_after_dollar = j + 1;
	else
		pos_after_dollar = j;
	var = ft_substr(str, pos_after_dollar, var_len);
	return (var);
}

//  && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'
// 	|| (str[i + 1] == '{' && str[i + 2] == '?' && str[i + 3] == '}')))
char	**find_dollar_sign(char *str, char **vars)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	while(str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			if (str[i + 1] >= '0' && str[i + 1] <= '9')
				vars[k] = ft_substr(str, i + 1, 1);
			else
			{
				vars[k] = fill_multi_vars(str, i);
				if (!vars[k])
					free_vars_vals(vars, NULL);
			}
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

int    get_vals_and_tot_len(char *str, char **vals, char **vars, t_varlist **head_var, t_parser *parser)
{
    int		i;
    size_t	vars_len;
    size_t	vals_len;
    int		t_len;

    i = 0;
    vars_len = 0;
    vals_len = 0;
    while (vars[i])
    {
		vars_len += ft_strlen(vars[i]);
        vals[i] = check_same_var_in_varlist(vars[i], head_var);
		if (!vals[i] && ft_strcmp(vars[i], "?") == 0)
			vals[i] = ft_itoa(*parser->g_exit_status);
		else if (!vals[i])
			vals[i] = ft_strdup("");
        vals_len += ft_strlen(vals[i]);
        i++;
    }
    vals[i] = NULL;
	t_len = ft_strlen(str) - if_dollar_sign(str) - vars_len + vals_len;
    return (t_len);
}

char	*reg_dollar_sign(char *str, t_varlist **head_var, t_parser *parser)
{
	int		nb_vars;
	char	*word;

	word = NULL;
	nb_vars = if_dollar_sign(str);
	if (nb_vars == 0)
		return (ft_strdup(str));
	word = replace_init_val_by_real_val(head_var, parser, nb_vars, str);
	if (!word)
		return (NULL);
	return (word);
}
