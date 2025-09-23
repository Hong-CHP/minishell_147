#include "minishell.h"
#include "libft.h"

char	**extract_path(t_varlist **head_var)
{
	t_varlist	*cur;
	char	**ev_path;

	cur = *head_var;
	while (cur)
	{
		if (ft_strcmp("PATH", cur->var_data->var) == 0)
		{
			ev_path = find_sign_then_split(cur->var_data->val);
			if (!ev_path)
			{
				perror("Extract path no success");
				free_split(ev_path);
				return (NULL);
			}
			return (ev_path);
		}
		cur = cur->next;
	}
	return (NULL);
}

char	*get_path_after_join(char **args, char **ev_path, int i)
{
	char	*temp;
	char	*path;

	temp = NULL;
	path = NULL;
	temp = ft_strjoin("/", args[0]);
	if (!temp)
	{
		free_split(ev_path);
		return (NULL);
	}
	path = ft_strjoin(ev_path[i], temp);
	free(temp);
	return (path);
}

char *search_in_path(char **args, char **ev_path)
{
    int   i;
    char *path;
    char *exe_path;

    exe_path = NULL;
    i = 0;
    while (ev_path[i])
    {
        path = get_path_after_join(args, ev_path, i);
        if (path)
        {
            if (access(path, X_OK) == 0)
            {
                exe_path = ft_strdup(path);
                free(path);
                break;
            }
            free(path);
        }
        i++;
    }
    return (exe_path);
}

char	*find_exe_path(char **args, t_varlist **head_var)
{
	char	*exe_path;
	char	**ev_path;

	exe_path = NULL;
	ev_path = extract_path(head_var);
	if (!ev_path)
		return (NULL);
	exe_path = search_in_path(args, ev_path);
	free_split(ev_path);
	return (exe_path);
}

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

int		no_executable_if_empty(t_command *cmd, t_parser *parser)
{
	if (!cmd)
		return (0);
	if (cmd->cmd && !*cmd->cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (cmd->invalid_in)
	{
		ft_put_err_msg(parser, NULL, cmd->infile);
		return (0);
	}
	return (1);
}

int		no_executable_if_invalid_in(t_command *cmd, t_parser *parser)
{
	t_redir	*tmp;

	if (cmd->outfile)
	{
		tmp = cmd->outfile;
		while (tmp->next)
		{
			if (tmp->invalid)
			{
				ft_put_err_msg(parser, NULL, tmp->name);
				return (0);
			}
			tmp = tmp->next;
		}
	}
	return (1);
}

void	build_in_single_cmd(t_varlist **head_var, t_command *cmd, t_pipex **pipe_data, t_parser *parser)
{
	int saved_stdin;
	int saved_stdout;
		
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin  = dup(STDIN_FILENO);
	if ((*pipe_data)->f_fds[0] != -1)
	{
	    dup2((*pipe_data)->f_fds[0], STDIN_FILENO);
	    close((*pipe_data)->f_fds[0]);
	}
	if ((*pipe_data)->f_fds[1] != -1)
	{
	    dup2((*pipe_data)->f_fds[1], STDOUT_FILENO);
	    close((*pipe_data)->f_fds[1]);
	}
	*parser->g_exit_status = execute_builtin(head_var, cmd, (*pipe_data)->envp, 0);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	return ;
}

void	single_cmd_child_redir(t_pipex **pipe_data)
{
	if ((*pipe_data)->f_fds[0] != -1)
	{
		dup2((*pipe_data)->f_fds[0], 0);
		close((*pipe_data)->f_fds[0]);
	}
	if ((*pipe_data)->f_fds[1] != -1)
	{
		dup2((*pipe_data)->f_fds[1], 1);
		close((*pipe_data)->f_fds[1]);
	}
}

int		executable_not_build_in(t_varlist **head_var, t_command *cmd, t_pipex *pipe_data, t_parser *parser)
{
	if (!no_executable_if_empty(cmd, parser))
		return (0);
	if (!no_executable_if_invalid_in(cmd, parser))
		return (0);	
	if (if_buildin(cmd->args[0]))
	{
		build_in_single_cmd(head_var, cmd, &pipe_data, parser);
		return (0);
	}
	return (1);
}

void	execute_single_cmd(t_varlist **head_var, t_command *cmd, t_pipex *pipe_data, t_parser *parser)
{
	pid_t	pid;
	// int		status;

	if (!executable_not_build_in(head_var, cmd, pipe_data, parser))
		return ;
	pid = fork();
	// status = 0;
	if (pid < 0)
	{
		perror("fork failed");
		*parser->g_exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		// signal(SIGINT, SIG_DFL);
		single_cmd_child_redir(&pipe_data);
		execute_cmd(head_var, cmd, pipe_data->envp, parser);
	}
	else
	{
		catch = 1;
		// signal(SIGINT, SIG_IGN);
		// waitpid(pid, &status, 0);
		// update_exit_status(status, parser);
		*parser->g_exit_status = wait_child_process(pid);
	}
}
