#include "minishell.h"
#include "libft.h"

void	child_process_redir_in(t_cmdlist *cur, t_pipex *pipe_data)
{
	int	infd;

	if (cur->command->here_doc == 1)
	{
		dup2(pipe_data->f_fds[0], STDIN_FILENO);
		close(pipe_data->f_fds[0]);
	}
	else if (cur->command->infile)
	{
		infd = open(cur->command->infile, O_RDONLY);
		if (infd < 0)
			exit(1);
		dup2(infd, STDIN_FILENO);
		close(infd);
	}
	else if (pipe_data->prev_pipe != -1)
	{
		dup2(pipe_data->prev_pipe, 0);
		close(pipe_data->prev_pipe);
	}
}

void	child_process_redir_out(t_cmdlist *cur, t_pipex *pipe_data)
{
	int		outfd;
	t_redir	*tmp;

	tmp = cur->command->outfile;
	while (tmp->next)
	{
		if (tmp->invalid)
			exit(1);
		tmp = tmp->next;
	}
	if (tmp->invalid)
		exit(1);
	if (tmp->append)
		outfd = open(tmp->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		outfd = open(tmp->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfd < 0)
		exit(1);
	dup2(outfd, STDOUT_FILENO);
	close(outfd);
	if (cur->next == NULL && pipe_data->pipefd[1] != -1)
		close(pipe_data->pipefd[1]);
}

void	child_process(t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data, t_parser *parser)
{
	if (cur->command->invalid_in)
		exit(1);
	child_process_redir_in(cur, pipe_data);
	if (cur->command->here_doc == 1)
	{
		dup2(pipe_data->f_fds[1], STDOUT_FILENO);
		close(pipe_data->f_fds[1]);
	}
	else if (cur->command->outfile)
		child_process_redir_out(cur, pipe_data);
	else if (cur->next != NULL)
	{
		dup2(pipe_data->pipefd[1], 1);
		close(pipe_data->pipefd[1]);
	}
	if (pipe_data->pipefd[0] != -1)
		close(pipe_data->pipefd[0]);
	execute_cmd(&(*head_cmd)->var_list, cur->command, pipe_data->envp, parser);
}

void	parent_process(t_cmdlist *cur, t_pipex *pipe_data)
{
	if (pipe_data->prev_pipe != -1)
		close(pipe_data->prev_pipe);
	if (cur->next != NULL)
	{
		close(pipe_data->pipefd[1]);
		pipe_data->prev_pipe = pipe_data->pipefd[0];
	}
	else
	{
		if (pipe_data->pipefd[0] != -1)
			close(pipe_data->pipefd[0]);
	}
}
