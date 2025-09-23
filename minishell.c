#include "minishell.h"
#include "libft.h"

t_parser	*init_parser(char *input, int *g_exit_status)
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
	parser->g_exit_status = g_exit_status;
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

int		parsing_minishell(t_parser **ps, t_cmdlist **hd_cmd, t_varlist **hd_var)
{
	*hd_cmd = parse_cmd_line(*ps, hd_var);
	if (!*hd_cmd)
	{
		ft_put_err_msg(*ps, NULL, NULL);
		return (0);
	}
	return (1);
}

void	minishell(char *input, t_varlist **head_var, char **envp, int *g_exit)
{
	t_parser	*parser;
	t_cmdlist	*head_cmd;
	t_pipex		*pipe_data;
	
	
	if (!input || !*input)
		return ;
	parser = NULL;
	pipe_data = NULL;
	parser = init_parser(input, g_exit);
	if (!parser)
		return ;
	if (!parsing_minishell(&parser, &head_cmd, head_var))
		return (clean_all(&head_cmd, parser, pipe_data));
	if ((ft_strchr(input, '=') && is_valide_varname(input)))
	{
		create_var_list_or_find_node(head_var, &head_cmd, parser);
		return (clean_all(&head_cmd, parser, pipe_data));
	}
	pipe_data = init_pipe_data(envp);
	if (!pipe_data)
		return (clean_all(&head_cmd, parser, pipe_data));
	execute_cmd_or_cmds(head_var, &head_cmd, pipe_data, parser);
	clean_all(&head_cmd, parser, pipe_data);
}

// void	print_fill_result(t_varlist **head_var, t_cmdlist **head_cmd, t_token **tok)
// {
// 	(void)head_var;
	// t_varlist *cur_var;
	// cur_var = (*head_cmd)->var_list;
	// while (cur_var)
	// {
	// 	if (cur_var->var_data)
	// 			printf("cur_var_var data var: %s (val: %s)\n", cur_var->var_data->var, cur_var->var_data->val);	
	// 	cur_var = cur_var->next;
	// }
// 	t_cmdlist *cur;
// 	cur = *head_cmd;
// 	int pipe = 0;
// 	while(cur)
// 	{
// 		pipe++;
// 		if (cur->command->args)
// 		{
// 		    for (int i = 0; i < cur->command->argc; i++)
// 		        printf("cur command args[%d] = %s\n", i, cur->command->args[i]);
// 		}
// 		else
//     		printf("cur node has null data\n");
// 		cur = cur->next;
// 	}
// 	printf("pipe number is %d\n", pipe);
// 	t_token *cur_t;
// 	int count = 0;
// 	cur_t = *tok;
// 	while(cur_t)
// 	{
// 		count++;
// 		printf("cur_t type: %d, value : %s\n", cur_t->type, cur_t->value);
// 		cur_t = cur_t->next;
// 	}
// 	printf("token size is %d\n", count);
// }