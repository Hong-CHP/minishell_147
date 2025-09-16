#include "minishell.h"
#include "libft.h"

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

t_pipex	*init_pipe_data(t_pipex *pipe_data, char **envp)
{
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
	// t_redir		*cur_redir;

	// if (cur->command->outfile)
	// {
	// 	cur_redir = cur->command->outfile;
	// 	while (cur_redir)
	// 	{
	// 		if (cur_redir->append)
	// 			pipe_data->f_fds[1] = open(cur_redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// 		else
	// 			pipe_data->f_fds[1] = open(cur_redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// 		if (pipe_data->f_fds[1] < 0)
	// 		{
	// 			check_outfile_permission(parser, cur_redir->name);
	// 			if (pipe_data->f_fds[0] > 0)
	// 				close(pipe_data->f_fds[0]);
	// 			ft_put_err_msg(parser, NULL, cur_redir->name);
	// 			return (0);
	// 		}
	// 		cur_redir = cur_redir->next;
	// 	}
	// }
	return (1);
}

int		get_in_out_files_fd(t_cmdlist **head, t_pipex *pipe_data, t_parser *parser)
{
	t_token *cur;
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
	// t_cmdlist	*cur;

	// cur = *head;
	// while (cur)
	// {
	// 	int	i = 0;
	// 	while (cur->command->cmd[i])
	// 	{
	// 		if (cur->command->cmd[i] == '>')
	// 		{
	// 			if (!check_and_open_outfile(cur, pipe_data, parser))
	// 				return (0);
	// 			if (!check_and_open_infile(cur, pipe_data, parser))
	// 				return (0);
	// 		}
	// 		if (cur->command->cmd[i] == '<')
	// 		{
	// 			if (!check_and_open_infile(cur, pipe_data, parser))
	// 				return (0);
	// 			if (!check_and_open_outfile(cur, pipe_data, parser))
	// 				return (0);
	// 		}
	// 		i++;
	// 	}
	// 	cur = cur->next;
	// }
	return (1);
}
