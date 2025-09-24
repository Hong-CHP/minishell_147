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
	if (if_still_space(&content[i]))
		split_space_in_val(&content[i], val);
	else
	{
		j = 0;
		while (content[i])
		{
			val[j] = content[i];
			j++;
			i++;
		}
		val[j] = '\0';
	}
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

int 	init_registre_variable(t_variable *var_dt, char *input)
{
	int		ch;

	if (if_export_variable(input))
		var_dt->exported = 1;
	if (!verify_and_init_var_val(input, var_dt))
		return (0);
	if (var_dt->exported == 1)
		fill_variable_value(&input[7], var_dt->var, var_dt->val);
	else
		fill_variable_value(input, var_dt->var, var_dt->val);
	ch = if_quote(var_dt->val);
	if (ch == -1)
	{
		printf("unclose quote\n");
		free(var_dt->var);
		free(var_dt->val);
		return (0);
	}
	return (1);
}

void	replace_new_val_for_val(char *find_val, t_variable *modif_var_node)
{
	if (if_still_space(find_val))
	{
		modif_var_node->val = malloc(sizeof(char) * (ft_strlen(find_val) + 1));
		if (!modif_var_node->val)
			return ;
		split_space_in_val(find_val, modif_var_node->val);
		free(find_val);
	}
	else
		modif_var_node->val = find_val;
}

// char	*extract_value(char *val, t_varlist **head_var)
// {
// 	char	*value;
// 	value = extract_value_if_sign(val, head_var);
// 	if (!value)
// 		return (NULL);
// 	return (value);
// }
// t_variable	*registre_var_val(char *input, t_variable *var_dt, char *value, t_varlist **head_var)
// {
// 	int		ch;
// 	// char	*tmp;
// 	(void)value;
// 	(void)head_var;
// 	if (var_dt->exported == 1)
// 		fill_variable_value(&input[7], var_dt->var, var_dt->val);
// 	else
// 		fill_variable_value(input, var_dt->var, var_dt->val);
// 	ch = if_quote(var_dt->val);
// 	if (ch == -1)
// 	{
// 		printf("unclose quote\n");
// 		free(var_dt->val);
// 		return (NULL);
// 	}
// 	return (var_dt);
// }