#include "minishell.h"
#include "libft.h"

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

	if (!executable_not_build_in(head_var, cmd, pipe_data, parser))
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		*parser->g_exit_status = 1;
		return ;
	}
	if (pid == 0)
	{	
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		single_cmd_child_redir(&pipe_data);
		execute_cmd(head_var, cmd, pipe_data->envp, parser);
	}
	else
	{
		catch = 1;
		*parser->g_exit_status = wait_child_process(pid);
		catch = 0;
	}
}

// signal(SIGINT, SIG_DFL);
// signal(SIGINT, SIG_IGN);
// waitpid(pid, &status, 0);
// update_exit_status(status, parser);
