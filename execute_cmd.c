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

void	check_each_path_per_slash(char *t_path, t_parser *parser)
{
	char	*tmp_path;
	char	*part;
	char	*last_slash;
	char	*end;

	tmp_path = t_path;
	part = NULL;
	end = NULL;
	last_slash = ft_strchr(tmp_path, '/');
	while (last_slash)
	{
		end = ft_strchr(last_slash + 1, '/');
		if (end == NULL)
			break ;
		part = ft_substr(tmp_path, 0, end - tmp_path);
		if (!check_dir_validate(part))
		{
			free(part);
			set_error(parser, "Not a directory", 126);
			ft_put_err_msg(parser, NULL, t_path);
			exit(126);
		}
		free(part);
		last_slash = end;
	}
}

void	path_exist_issue(t_command *cmd, int *emp, t_parser *p, char *ex_path)
{
	if (if_slash(cmd->args[*emp]) > 0 && !ex_path)
	{
		check_each_path_per_slash(cmd->args[*emp], p);
		set_error(p, "No such file or directory", 127);
		ft_put_err_msg(p, NULL, cmd->args[*emp]);
		exit(127);
	}
	if (!ex_path)
	{
		set_error(p, "command not found", 127);
		ft_put_err_msg(p, NULL, cmd->args[*emp]);
		exit (127);
	}
}

void	slash_in_path(t_command *cmd, int *emp, t_parser *p, char **ex_path)
{
	if (access(cmd->args[*emp], X_OK) == 0)
	{
		*ex_path = ft_strdup(cmd->args[*emp]);
	}
	else
	{
		if (access(cmd->args[*emp], F_OK) == 0)
		{
			set_error(p, "Permission denied", 126);
			ft_put_err_msg(p, NULL, cmd->args[*emp]);
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
		slash_in_path(command, &empty, parser, &exe_path);
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
