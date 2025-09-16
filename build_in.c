#include "minishell.h"
#include "libft.h"

int		if_buildin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	return (0);
}

int		builtin_unset(t_varlist **head_var, t_command *cmd_node)
{
	t_varlist	*cur;
	t_varlist	*prev;
	t_varlist	*tmp;
	int			i;

	i = 1;
	while (i < cmd_node->argc)
	{
		cur = *head_var;
		prev = NULL;
		while (cur)
		{
			if (ft_strcmp(cur->var_data->var, cmd_node->args[i]) == 0
				&& cur->var_data->exported == 1)
			{
				if (prev)
					prev->next = cur->next;
				else
					*head_var = cur->next;
				tmp = cur->next;
				free(cur->var_data->val);
				free(cur->var_data->var);
				free(cur->var_data);
				free(cur);
				cur = tmp;
				continue;
			}
			prev = cur;
			cur = cur->next;
		}
		i++;
	}
	return (0);
}

int		builtin_export(t_varlist **head_var, t_command *cmd_node, int sub_process)
{
	t_variable	*var_node;
	int			i;

	var_node = NULL;
	if (cmd_node->argc == 1)
	{
		if (ft_strchr(cmd_node->cmd, '"'))
		{
			ft_putstr_fd("minishell:", 2);
			ft_putstr_fd(cmd_node->cmd, 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (1);
		}
		print_all_variable_in_list(head_var);
	}
	else if (!sub_process)
	{
		i = 1;
		while (i < cmd_node->argc)
		{
			if (process_var_val_export(head_var, cmd_node->args[i], var_node, cmd_node) == 1)
			{
				ft_putstr_fd("minishell:", 2);
				ft_putstr_fd(cmd_node->args[i], 2);
				ft_putstr_fd(": not a valid identifier\n", 2);
				return (1);
			}
			i++;
		}
	}
	return (0);
}

int		execute_builtin(t_varlist **head_var, t_command *cmd_node, char **ev, int sub_process)
{
	(void)ev;
	if (!cmd_node || !cmd_node->args)
		return (0);
	if (ft_strcmp(cmd_node->args[0], "cd") == 0)
		return (builtin_cd(cmd_node));
	if (ft_strcmp(cmd_node->args[0], "echo") == 0)
		return (builtin_echo(cmd_node));
	if (ft_strcmp(cmd_node->args[0], "export") == 0)
		return (builtin_export(head_var, cmd_node, sub_process));
	if (ft_strcmp(cmd_node->args[0], "unset") == 0)
	{
		if (!cmd_node->args[1] || cmd_node->args[1][0] == '\0')
			return (0);
		return (builtin_unset(head_var, cmd_node));
	}
	if (ft_strcmp(cmd_node->args[0], "env") == 0)
		return (builtin_env(ev, head_var));
	if (ft_strcmp(cmd_node->args[0], "exit") == 0)
		return (builtin_exit(cmd_node));
	if (ft_strcmp(cmd_node->args[0], "pwd") == 0)
		return (builtin_pwd());
	return (1);
}