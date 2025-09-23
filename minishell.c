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
void	free_cmdlist_content(t_cmdlist **cur)
{
	if ((*cur)->command->args)
	{
		while ((*cur)->command->argc >= 0)
		{
			free((*cur)->command->args[(*cur)->command->argc]);
			(*cur)->command->argc--;
		}
		free((*cur)->command->args);
	}
	if ((*cur)->command->infile)
		free((*cur)->command->infile);
	if ((*cur)->command->limiter)
		free((*cur)->command->limiter);
	if ((*cur)->command->outfile)
		free_redir(&(*cur)->command->outfile);
	free((*cur)->command);
}

void	free_cmdlist(t_cmdlist **head_cmd)
{
	t_cmdlist *cur;
	t_cmdlist *next;

	cur = *head_cmd;
	while (cur)
	{
		next = cur->next;
		if (cur->command)
			free_cmdlist_content(&cur);
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
		{
			free(current->value);
			current->value = NULL;
		}
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

void	execute_cmd_or_cmds(t_varlist **head_var, t_cmdlist **head_cmd, t_pipex	*pipe_data, t_parser *parser)
{
	if ((*head_cmd)->command->here_doc == 1)
	{
		if (!execute_here_doc(head_cmd, pipe_data, parser))
			return ;
	}
	else
	{
		if (if_pipex(head_cmd) > 1)
			get_in_out_files_fd(head_cmd, pipe_data, parser);
		else
		{
			if (!get_in_out_files_fd(head_cmd, pipe_data, parser))
				return ;
		}
	}
	if (if_pipex(head_cmd) > 1)
		execute_pipeline(head_cmd, pipe_data, parser);
	else
		execute_single_cmd(head_var, (*head_cmd)->command, pipe_data, parser);
	if ((*head_cmd)->command->here_doc == 1)
		unlink("here.txt");
}

	// if (if_pipex(head_cmd) > 1)
	// {
	// 	if ((*head_cmd)->command->here_doc == 1)
	// 	{
	// 		if (!execute_here_doc(head_cmd, pipe_data, parser))
	// 			return ;
	// 	}
	// 	else
	// 		get_in_out_files_fd(head_cmd, pipe_data, parser);
	// 	execute_pipeline(head_cmd, pipe_data, parser);
	// 	if ((*head_cmd)->command->here_doc == 1)
	// 		unlink("here.txt");
	// }
	// else
	// {
	// 	if ((*head_cmd)->command->here_doc == 1)
	// 	{
	// 		if (!execute_here_doc(head_cmd, pipe_data, parser))
	// 			return ;
	// 	}
	// 	else
	// 		if (!get_in_out_files_fd(head_cmd, pipe_data, parser))
	// 			return ;
	// 	execute_single_cmd(head_var, (*head_cmd)->command, pipe_data, parser);
	// 	if ((*head_cmd)->command->here_doc == 1)
	// 		unlink("here.txt");
	// }
// }

int		parsing_minishell(char *input, t_parser **ps, t_cmdlist **hd_cmd, t_varlist **hd_var)
{
	*ps = init_parser(input);
	if (!*ps)
		return (0);
	*hd_cmd = parse_cmd_line(*ps, hd_var);
	if (!*hd_cmd)
	{
		ft_put_err_msg(*ps, NULL, NULL);
		return (0);
	}
	return (1);
}

void	clean_all(t_cmdlist **head_cmd, t_parser *parser, t_pipex *pipe_data)
{
	if (head_cmd || *head_cmd)
		free_cmdlist(head_cmd);
	if (parser)
		free_parser(parser);
	if (pipe_data)
		free(pipe_data);
}

void	minishell(char *input, t_varlist **head_var, char **envp)
{
	t_parser	*parser;
	t_cmdlist	*head_cmd;
	t_pipex		*pipe_data;

	if (!input || !*input)
		return ;
	parser = NULL;
	pipe_data = NULL;
	if (!parsing_minishell(input, &parser, &head_cmd, head_var))
	{
		clean_all(&head_cmd, parser, pipe_data);
		return ;
	}
	if ((ft_strchr(input, '=') && is_valide_varname(input)))
	{
		create_var_list_or_find_node(head_var, input, &head_cmd, parser);
		clean_all(&head_cmd, parser, pipe_data);
		return ;
	}
	pipe_data = init_pipe_data(envp);
	if (!pipe_data)
	{
		clean_all(&head_cmd, parser, pipe_data);
		return ;
	}
	execute_cmd_or_cmds(head_var, &head_cmd, pipe_data, parser);
	clean_all(&head_cmd, parser, pipe_data);
}
