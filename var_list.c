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

void    find_var_node_modif_val(t_variable *modif_var_node, char *match, t_varlist **head)
{
    char *find_val;
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
		free(modif_var_node->val);
		free(find_val);
		return ;
	}
	if (ch != 0)
	{
		find_val = extract_value(find_val, head);
		if (!find_val)
		{
			free(modif_var_node->val);
			return ;
		}
	}
    if (modif_var_node->val)
        free(modif_var_node->val);
    modif_var_node->val = find_val;
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
	if (!init_registre_variable(var_node->var_data, input, head))
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

	if (!is_valide_varname(input))
		return (1);
	if (ft_strchr(input, '='))
	{
		match_var = ft_strchr(input, '=');
		find_var = ft_substr(input, 0, match_var - input);
	}
	else
		find_var = ft_substr(input, 0, ft_strlen(input));
	// printf("find_var is %s\n", find_var);
	var_node = search_target_var_node(head, find_var);
	if (!var_node)
		create_var_list(head, input, cmd_node);
	else if (var_node && ft_strchr(input, '='))
	{
		find_var_node_modif_val(var_node, match_var, head);
		var_node->exported = 1;
	}
	else if (var_node && !ft_strchr(input, '='))
		var_node->exported = 1;
	if (find_var)
		free(find_var);
	return (0);
}

int		create_var_list_or_find_node(t_varlist **head, char *input, t_cmdlist **head_cmd)
{
	char		*match_var;
	char		*find_var;
	t_variable  *var_node;
	t_cmdlist	*cur;
	int			i;
	(void)input;

	var_node = NULL;
	match_var = NULL;
	find_var = NULL;
	cur = *head_cmd;
	while (cur)
	{
		i = 0;
		while (i < cur->command->argc)
		{
			if (ft_strchr(cur->command->args[i], '='))
			{
				if (ft_list_size(head) > 0)
				{
					match_var = ft_strchr(cur->command->args[i], '=');
					find_var = ft_substr(cur->command->args[i], 0, match_var - cur->command->args[i]);
					var_node = search_target_var_node(head, find_var);
					if (!var_node)
						create_var_list(head, cur->command->args[i], NULL);
					else
						find_var_node_modif_val(var_node, match_var, head);
					free(find_var);
				}
				else
					create_var_list(head, cur->command->args[i], NULL);
			}
			else
				minishell(cur->command->args[i], head, NULL);
			i++;
		}
		cur = cur->next;
	}
	return (0);
}
