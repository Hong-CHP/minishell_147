#include "minishell.h"
#include "libft.h"

int		put_export_err_msg(t_command *cmd_node)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_node->args[1], 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	return (1);
}

t_varlist	*create_export_list(t_varlist **export_head, t_varlist **head)
{
	t_varlist	*export_node;
	t_varlist	*cur;

	cur = *head;
	while (cur)
	{
		export_node = malloc(sizeof(t_varlist));
		if (!export_node)
			return (NULL);
		export_node->var_data = malloc(sizeof(t_variable));
		if (!export_node->var_data)
		{
			free(export_node);
			return (NULL);
		}
		export_node->var_data->var = ft_strdup(cur->var_data->var);
		export_node->var_data->val = ft_strdup(cur->var_data->val);
		export_node->var_data->exported = cur->var_data->exported;
		add_var_lst_order(export_head, export_node);
		cur = cur->next;
	}
	return (*export_head);
}

int		get_cd_path(t_command *cmd, char **path, t_varlist **head)
{
	if (cmd->argc == 1)
	{
		if (get_cd_home_path(path, head) != 0)
			return (1);
	}
	else
	{
		if (cmd->args[1] && !cmd->args[1][0] && cmd->cmd[3] == '$')
		{
			if (get_cd_home_path(path, head) != 0)
				return (1);
		}
		else if (cmd->args[1] && !cmd->args[1][0])
		{
			*path = ft_strdup(".");
			return (0);
		}
		else
			*path = cmd->args[1];
	}
	return (0);
}

void	put_echo_content(t_command *cmd, int *i)
{
	while (cmd->args[*i])
	{
		ft_putstr_fd(cmd->args[*i], 1);
		if (*i < cmd->argc - 1)
			ft_putstr_fd(" ", 1);
		(*i)++;
	}
}

int		check_echo_n_in_args(char *str)
{
	int	i;

	if (ft_strncmp(str, "-n", 2) == 0)
		i = 2;
	else
		i = 0;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
