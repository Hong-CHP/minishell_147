#include "minishell.h"
#include "libft.h"

t_variable	*search_target_var_node(t_varlist **head, char *find_var)
{
	t_varlist	*cur;

	cur = *head;
	while (cur)
	{
		if (ft_strcmp(cur->var_data->var, find_var) == 0)
			return (cur->var_data);
		cur = cur->next;
	}
	return (NULL);
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

void    find_var_node_modif_val(t_variable *modif_var_node, char *match)
{
    char	*find_val;
	int		ch;

	if (*(match + 1))
    	find_val = ft_strdup(match + 1);
	else
		find_val = ft_strdup("");
    if (!find_val)
	{
        return ;
	}
	ch = if_quote(find_val);
	if (ch == -1)
	{
		printf("unclose quote\n");
		free(find_val);
		return ;
	}
	if (modif_var_node->val)
        free(modif_var_node->val);
	replace_new_val_for_val(find_val, modif_var_node);
}

void	create_var_list(t_varlist **head, char *input, t_command *cmd_node)
{
	t_varlist	*var_node;
	
	var_node = malloc(sizeof(t_varlist));
	if (!var_node)
		return ;
	ft_memset(var_node, 0, sizeof(t_varlist));
	var_node->var_data = malloc(sizeof(t_variable));
	if (!var_node->var_data)
	{
		free(var_node);
		return ;
	}
	ft_memset(var_node->var_data, 0, sizeof(t_variable));
	if (!init_registre_variable(var_node->var_data, input))
	{
		free(var_node->var_data);
		free(var_node);
		return ;
	}
	if (cmd_node && ft_strcmp(cmd_node->args[0], "export") == 0)
		var_node->var_data->exported = 1;
	var_node->next = NULL;
	add_var_lst_back(head, var_node);
}

int		process_var_val_export(t_varlist **head, char *input, t_variable *var_node, t_command *cmd_node)
{
	char *match_var;
	char   *find_var;

	if (!is_valide_varname_for_export(input, head, cmd_node))
		return (1);
	if (ft_strchr(input, '='))
	{
		match_var = ft_strchr(input, '=');
		find_var = ft_substr(input, 0, match_var - input);
	}
	else
		find_var = ft_substr(input, 0, ft_strlen(input));
	var_node = search_target_var_node(head, find_var);
	if (!var_node)
		create_var_list(head, input, cmd_node);
	else if (var_node && ft_strchr(input, '='))
	{
		find_var_node_modif_val(var_node, match_var);
		var_node->exported = 1;
	}
	else if (var_node && !ft_strchr(input, '='))
		var_node->exported = 1;
	if (find_var)
		free(find_var);
	return (0);
}

void	reg_variable_list_issue(char *input, t_varlist **head)
{
	char		*match_var;
	char		*find_var;
	t_variable  *var_node;

	var_node = NULL;
	match_var = NULL;
	find_var = NULL;
	match_var = ft_strchr(input, '=');
	find_var = ft_substr(input, 0, match_var - input);
	var_node = search_target_var_node(head, find_var);
	if (!var_node)
		create_var_list(head, input, NULL);
	else
		find_var_node_modif_val(var_node, match_var);
	free(find_var);
}

int		create_var_list_or_find_node(t_varlist **head, t_cmdlist **head_cmd, t_parser *parser)
{
	t_cmdlist	*cur;
	int			i;
	
	cur = *head_cmd;
	while (cur)
	{
		i = 0;
		while (i < cur->command->argc)
		{
			if (ft_strchr(cur->command->args[i], '='))
				reg_variable_list_issue(cur->command->args[i], head);
			else
				minishell(cur->command->args[i], head, NULL, parser->g_exit_status);
			i++;
		}
		cur = cur->next;
	}
	return (0);
}
