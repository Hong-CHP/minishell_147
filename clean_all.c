#include "minishell.h"
#include "libft.h"

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
	t_cmdlist	*cur;
	t_cmdlist	*next;

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

void	clean_all(t_cmdlist **head_cmd, t_parser *parser, t_pipex *pipe_data)
{
	if (head_cmd || *head_cmd)
		free_cmdlist(head_cmd);
	if (parser)
		free_parser(parser);
	if (pipe_data)
		free(pipe_data);
}
