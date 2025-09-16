#include "minishell.h"
#include "libft.h"

void	print_fill_result(t_varlist **head_var, t_cmdlist **head_cmd, t_token **tok)
{
	(void)head_var;
	// t_varlist *cur_var;
	// cur_var = (*head_cmd)->var_list;
	// while (cur_var)
	// {
	// 	if (cur_var->var_data)
	// 			printf("cur_var_var data var: %s (val: %s)\n", cur_var->var_data->var, cur_var->var_data->val);	
	// 	cur_var = cur_var->next;
	// }
	t_cmdlist *cur;
	cur = *head_cmd;
	int pipe = 0;
	while(cur)
	{
		pipe++;
		if (cur->command->args)
		{
		    for (int i = 0; i < cur->command->argc; i++)
		        printf("cur command args[%d] = %s\n", i, cur->command->args[i]);
		}
		else
    		printf("cur node has null data\n");
		cur = cur->next;
	}
	printf("pipe number is %d\n", pipe);
	t_token *cur_t;
	int count = 0;
	cur_t = *tok;
	while(cur_t)
	{
		count++;
		printf("cur_t type: %d, value : %s\n", cur_t->type, cur_t->value);
		cur_t = cur_t->next;
	}
	printf("token size is %d\n", count);

}

void	free_cmdlist(t_cmdlist **head_cmd)
{
	t_cmdlist *cur;
	t_cmdlist *next;

	cur = *head_cmd;
	while (cur)
	{
		next = cur->next;
		if (cur->command && cur->command->args)
		{
			while (cur->command->argc >= 0)
			{
				free(cur->command->args[cur->command->argc]);
				cur->command->argc--;
			}
			free(cur->command->args);
		}
		if (cur->command->infile)
			free(cur->command->infile);
		if (cur->command->outfile)
			free(cur->command->outfile);
		free(cur->command);
		free(cur);
		cur = next;
	}
}

void	free_parser(t_parser *parser)
{
	t_token	*current;
	t_token	*next;

	if (!parser)
		return ;
	if (parser->input)
		free(parser->input);
	if (parser->error_msg)
		free(parser->error_msg);
	current = parser->tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	free(parser);
}

t_parser	*init_parser(char *input)
{
	t_parser *parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	ft_memset(parser, 0, sizeof(t_parser));
	parser->input = ft_strdup(input);
	if (!parser->input)
	{
		free(parser);
		return (NULL);
	}
	parser->tokens = NULL;
	parser->current = NULL;
	parser->pos = 0;
	parser->error = 0;
	parser->error_msg = NULL;
	return (parser);
}

// int		check_and_open_all_redir(t_cmdlist **head, t_pipex *pipe_data, t_parser *parser)
// {
// 	t_cmdlist	*cur;
// 	t_redir		*cur_redir;
// 	int			fd;

// 	cur = *head;
// 	while (cur)
// 	{
// 		if (cur->command->infile)
// 		{
// 			pipe_data->f_fds[0] = open(cur->command->infile, O_RDONLY);
// 			if (pipe_data->f_fds[0] < 0)
// 			{
// 				check_infile_permission(parser, cur->command->infile);
// 				ft_put_err_msg(parser, NULL, cur->command->infile);
// 				return (0);
// 			}
// 		}
// 		if (cur->command->outfile)
// 		{
// 			cur_redir = cur->command->outfile;
// 			while (cur_redir)
// 			{
// 				if (cur_redir->append)
// 					fd = open(cur_redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 				else
// 					fd = open(cur_redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 				if (fd < 0)
// 				{
// 					check_outfile_permission(parser, cur_redir->name);
// 					ft_put_err_msg(parser, NULL,  cur_redir->name);
// 					return (0);
// 				}
// 				close(fd);
// 				cur_redir = cur_redir->next;
// 			}
// 		}
// 	}
// 	return (1);
// }

void	execute_cmd_or_cmds(t_varlist **head_var, t_cmdlist **head_cmd, t_pipex	*pipe_data, t_parser *parser)
{
	if (if_pipex(head_cmd) > 1)
	{
		if ((*head_cmd)->command->here_doc == 1)
		{
			if (!execute_here_doc(head_cmd, pipe_data))
				return ;
		}
		else
			get_in_out_files_fd(head_cmd, pipe_data, parser);
		// printf("fd[0]: %d, fd[1]: %d\n", pipe_data->f_fds[0], pipe_data->f_fds[1]);
		execute_pipeline(head_cmd, pipe_data, parser);
		if ((*head_cmd)->command->here_doc == 1)
			unlink("here.txt");
	}
	else
	{
		if ((*head_cmd)->command->here_doc == 1)
		{
			if (!execute_here_doc(head_cmd, pipe_data))
				return ;
		}
		else
			if (!get_in_out_files_fd(head_cmd, pipe_data, parser))
				return ;
		// printf("fd[0]: %d, fd[1]: %d\n", pipe_data->f_fds[0], pipe_data->f_fds[1]);
		execute_single_cmd(head_var, (*head_cmd)->command, pipe_data, parser);
		if ((*head_cmd)->command->here_doc == 1)
			unlink("here.txt");
	}
}

int		parsing_minishell(char *input, t_parser **ps, t_cmdlist **hd_cmd, t_varlist **hd_var)
{
	*ps = init_parser(input);
	if (!*ps)
		return (0);
	*hd_cmd = parse_cmd_line(*ps, hd_var);
	if (!*hd_cmd)
	{
		ft_put_err_msg(*ps, NULL, NULL);
		free_parser(*ps);
		return (0);
	}
	return (1);
}

void	minishell(char *input, t_varlist **head_var, char **envp)
{
	t_parser	*parser;
	t_cmdlist	*head_cmd;
	t_pipex		*pipe_data;

	if (!input || !*input)
		return ;
	parser = NULL;
	if (!parsing_minishell(input, &parser, &head_cmd, head_var))
		return ;
	if ((ft_strchr(input, '=') && is_valide_varname(input)))
	{
		create_var_list_or_find_node(head_var, input, &head_cmd);
		return ;
	}
	// print_fill_result(head_var, &head_cmd, &(parser->tokens));
	pipe_data = malloc(sizeof(t_pipex));
	if (!pipe_data)
	{
		free_parser(parser);
		return ;
	}
	init_pipe_data(pipe_data, envp);
	execute_cmd_or_cmds(head_var, &head_cmd, pipe_data, parser);
	free_cmdlist(&head_cmd);
	free_parser(parser);
	free(pipe_data);
}

// parser = init_parser(input);
// if (!parser)
// 	return;
// head_cmd = parse_cmd_line(parser, head_var);
// if (!head_cmd)
// {
// 	ft_put_err_msg(parser, NULL, NULL);
// 	free_parser(parser);
// 	return;
// }