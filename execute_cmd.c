#include "minishell.h"
#include "libft.h"

void	exit_if_empty(t_command *command, int *empty)
{
	if (!command->cmd)
		exit(0);
	if (!command->args[0])
	{
		(*empty)++;
		if (!command->args[*empty])
			exit(0);
	}	
}

void	path_exist_issue(t_command *command, int *empty, t_parser *parser, char *exe_path)
{
	if (if_slash(command->args[*empty]) > 0 && !exe_path)
	{
		set_error(parser, "No such file or directory", 127);
		ft_put_err_msg(parser, NULL, command->args[*empty]);
		exit(127);
	}
	if (!exe_path)
	{
		set_error(parser, "command not found", 127);
		ft_put_err_msg(parser, NULL, command->args[*empty]);
		exit (127);
	}
}

void	slash_already_path(t_command *command, int *empty, t_parser *parser, char **exe_path)
{
	if (access(command->args[*empty], X_OK) == 0)
	{
		*exe_path = ft_strdup(command->args[*empty]);
	}
	else
	{
		if (access(command->args[*empty], F_OK) == 0)
		{
			set_error(parser, "Permission denied", 126);
			ft_put_err_msg(parser, NULL, command->args[*empty]);
			exit (126);
		}
	}
}

void	execute_cmd(t_varlist **head_var, t_command *command, char **ev, t_parser *parser)
{
	char	*exe_path;
	int		empty;

	exe_path = NULL;
	empty = 0;
	exit_if_empty(command, &empty);
	if (if_buildin(command->args[empty]))
	{
		*parser->g_exit_status = execute_builtin(head_var, command, ev, 1);
		exit(*parser->g_exit_status);
	}
	if (if_slash(command->args[empty]) > 0)
		slash_already_path(command, &empty, parser, &exe_path);
	else
		exe_path = find_exe_path(command->args, head_var);
	path_exist_issue(command, &empty, parser, exe_path);
	if (exe_path)
	{
		execve(exe_path, command->args, ev);
		set_error(parser, "Is a directory", 126);
		ft_put_err_msg(parser, NULL, command->args[empty]);
		free(exe_path);
		exit(126);
	}
}
