#include "minishell.h"
#include "libft.h"

int	if_export_variable(char *content)
{
	if (ft_strncmp("export", content, 6) == 0)
		return (1);
	return (0);
}

int	is_valide_varname_for_export(char *input, t_varlist **head, t_command *cmd_node)
{
	int i;
	
	if (!input)
		return (0);
	if (!input[0] && cmd_node->cmd[7] == '$')
	{
		print_all_variable_in_list(head);
		return (1);
	}
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
