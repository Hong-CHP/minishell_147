#include "minishell.h"
#include "libft.h"

void	free_redir(t_redir **head)
{
	t_redir	*next;
	t_redir	*cur;

	cur = *head;
	while (cur)
	{
		next = cur->next;
		if (cur->name)
			free(cur->name);
		free(cur);
		cur = next;
	}
}

int		pipe_fork_error(void)
{
	perror("pipe");
	return (-1);
}

int		if_pipex(t_cmdlist **head_cmd)
{
	t_cmdlist	*cur;
	int			pipe; 
	
	pipe = 0;
	cur = *head_cmd;
	while(cur)
	{
		pipe++;
		cur = cur->next;
	}
	return (pipe);
}

t_pipex	*init_pipe_data(char **envp)
{
	t_pipex *pipe_data;

	pipe_data = malloc(sizeof(t_pipex));
	if (!pipe_data)
		return (NULL);
	pipe_data->f_fds[0] = -1;
	pipe_data->f_fds[1] = -1;
	pipe_data->envp = envp;
	pipe_data->prev_pipe = -1;
	return (pipe_data);
}

int		check_and_open_infile(t_cmdlist *cur, t_pipex *pipe_data, t_parser *parser)
{
	if (cur->command->infile)
	{
		pipe_data->f_fds[0] = open(cur->command->infile, O_RDONLY);
		if (pipe_data->f_fds[0] < 0)
		{
			check_infile_permission(parser, cur->command->infile);
			ft_put_err_msg(parser, NULL, cur->command->infile);
			return (0);
		}
	}
	return (1);
}

int		check_and_open_outfile(t_token *cur, t_pipex *pipe_data, t_parser *parser)
{
	if (cur->type == TOKEN_REDIR_APPEND)
		pipe_data->f_fds[1] = open(cur->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		pipe_data->f_fds[1] = open(cur->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipe_data->f_fds[1] < 0)
	{
		check_outfile_permission(parser, cur->next->value);
		if (pipe_data->f_fds[0] > 0)
			close(pipe_data->f_fds[0]);
		ft_put_err_msg(parser, NULL, cur->next->value);
		return (0);
	}
	return (1);
}

int		get_in_out_files_fd(t_cmdlist **head, t_pipex *pipe_data, t_parser *parser)
{
	t_token		*cur;
	t_cmdlist	*cur_cmd;

	cur_cmd = *head;
	while (cur_cmd)
	{
		cur = parser->tokens;
		while (cur)
		{
			if (cur->type == TOKEN_REDIR_IN)
			{
				if (!check_and_open_infile(cur_cmd, pipe_data, parser))
					return (0);
			}
			if (cur->type == TOKEN_REDIR_OUT || cur->type == TOKEN_REDIR_APPEND)
			{
				if (!check_and_open_outfile(cur, pipe_data, parser))
					return (0);
			}
			cur = cur->next;
		}
		cur_cmd = cur_cmd->next;
	}
	return (1);
}
