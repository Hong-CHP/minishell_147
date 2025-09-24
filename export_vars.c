#include "minishell.h"
#include "libft.h"

int		count_size_of_envp(char **envp)
{
	int	size;

	size = 0;
	while(envp[size])
		size++;
	return (size);
}

void	print_all_variable_in_list(t_varlist **head)
{
	t_varlist	*cur;
	t_varlist	*export_head;

	export_head = NULL;
	create_export_list(&export_head, head);
	if (!export_head)
		return ;
	cur = export_head;
	while (cur)
	{
		if (cur->var_data->exported == 1)
			printf("declare -x %s=\"%s\"\n", cur->var_data->var, cur->var_data->val);
		cur = cur->next;
	}
	clean_var_list(&export_head);
}

void	init_varnode_vardata_for_ev(t_varlist **var_node)
{
	*var_node = malloc(sizeof(t_varlist));
	if (!*var_node)
		return ;
	ft_memset(*var_node, 0, sizeof(t_varlist));
	(*var_node)->var_data = malloc(sizeof(t_variable));
	if (!(*var_node)->var_data)
	{
		free((*var_node));
		return ;
	}
	ft_memset((*var_node)->var_data, 0, sizeof(t_variable));
	(*var_node)->var_data->exported = 1;
}

void	create_var_list_for_ev(t_varlist **head, char *input)
{
	t_varlist	*var_node;
	int			len;
	int			i;
	
	init_varnode_vardata_for_ev(&var_node);
	var_node->var_data->var = if_variable_var(input);
	if (!var_node->var_data->var)
		return ;
	i = 0;
	len = 0;
	while (input[i] && input[i] != '=')
		i++;
	while (input[i])
	{
		len++;
		i++;
	}
	var_node->var_data->val = malloc(sizeof(char) * (len + 1));
	if (!var_node->var_data->val)
		return ;
	fill_variable_value(input, var_node->var_data->var, var_node->var_data->val);
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
