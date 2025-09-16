#include "minishell.h"
#include "libft.h"

void	fill_variable_value(char *content, char *var, char *val)
{
	int	i;
	int	j;

	i = 0;
	while (content[i] && content[i] != '=')
	{
		var[i] = content[i];
		i++;
	}
	var[i] = '\0';
	if (content[i] && content[i] == '=')
		i++;
	j = 0;
	while (content[i])
	{
		val[j] = content[i];
		j++;
		i++;
	}
	val[j] = '\0';
}

char	*extract_value(char *val, t_varlist **head_var)
{
	char	*value;

	value = extract_value_if_sign(val, head_var);
	if (!value)
		return (NULL);
	return (value);
}

t_variable	*registre_var_val(char *input, t_variable *var_dt, char *value, t_varlist **head_var)
{
	int		ch;
	char	*tmp;

	if (var_dt->exported == 1)
		fill_variable_value(&input[7], var_dt->var, var_dt->val);
	else
		fill_variable_value(input, var_dt->var, var_dt->val);
	ch = if_quote(var_dt->val);
	if (if_dollar_sign(var_dt->val) > 0 && ch == 0)
	{
		tmp = var_dt->val;
		var_dt->val = reg_dollar_sign(tmp, head_var);
		free(tmp);
	}
	if (ch == -1)
	{
		printf("unclose quote\n");
		free(var_dt->val);
		return (NULL);
	}
	if (ch != 0)
	{
		value = extract_value(var_dt->val, head_var);
		if (!value)
		{
			free(var_dt->val);
			return (NULL);
		}	
		free(var_dt->val);
		var_dt->val = value;
	}
	return (var_dt);
}

t_variable	*verify_and_init_var_val(char *input, t_variable *var_dt)
{
	if (var_dt->exported == 1)
		var_dt->var = if_variable_var(&input[7]);
	else
		var_dt->var = if_variable_var(input);
	if (!var_dt->var)
		return (NULL);
	var_dt->val = if_variable_val(input);
	if (!var_dt->val)
	{
		free(var_dt->var);
		return (NULL);
	}
	return (var_dt);
}

int 	init_registre_variable(t_variable *var_dt, char *input, t_varlist **head_var)
{
	char	*value;

	value = NULL;
	if (if_export_variable(input))
		var_dt->exported = 1;
	if (!verify_and_init_var_val(input, var_dt))
		return (0);
	if (!registre_var_val(input, var_dt, value, head_var))
		return (0);
	return (1);
}
