#include "minishell.h"
#include "libft.h"

int	handle_word(t_parser *p, char **args, int *argc)
{
	char	**split_word;
	int		i;
	char	*pos_quote;

	pos_quote = ft_strnstr(p->input, p->current->value, ft_strlen(p->input));
	if (p->input && (pos_quote - p->input - 1 > 0)
		&& p->input[pos_quote - p->input - 1] != '"'
		&& p->input && p->input[pos_quote - p->input - 1] != '\''
		&& if_still_space(p->current->value))
	{
		i = 0;
		split_word = ft_split(p->current->value, ' ');
		while (split_word[i])
		{
			args[*argc] = ft_strdup(split_word[i]);
			if (!args[*argc])
				return (1);
			if (!*args[*argc])
				return (0);
			(*argc)++;
			i++;
		}
		free_split(split_word);
	}
	else
	{
		args[*argc] = ft_strdup(p->current->value);
		if (!args[*argc])
			return (1);
		if (!*args[*argc])
			return (0);
		(*argc)++;
	}
	return (0);
}

int		handle_infile(t_parser *p, t_cmdlist *node)
{
	p->current = p->current->next;
	if (!p->current || p->current->type != TOKEN_WORD)
		return (set_error(p, "syntax error near unexpected token `newline'", 2));
	if (node->command->infile)
		free(node->command->infile);
	node->command->infile = ft_strdup(p->current->value);
	if (!node->command->infile)
		return (1);
	// printf("checking permission\n");
	if (check_infile_permission(p, node->command->infile) != 0)
	{
		// free(node->command->infile);
		// return (1);
		// printf("invalid_in = 1\n");
		node->command->invalid_in = 1;
	}
	return (0);
}

int		handle_outfile(t_parser *p, t_cmdlist *node, int append)
{
	t_redir	*new;
	t_redir	*cur;

	p->current = p->current->next;
	if (!p->current || p->current->type != TOKEN_WORD)
		return (set_error(p, "syntax error near unexpected token `newline'", 2));
	new = malloc(sizeof(t_redir));
	if (!new)
		return (1);
	new->name = ft_strdup(p->current->value);
	new->invalid = 0;
	new->append = append;
	new->next = NULL;
	if (!node->command->outfile)
		node->command->outfile = new;
	else
	{
		cur = node->command->outfile;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	// printf("checking permission\n");
	if (check_outfile_permission(p, new->name) != 0)
	{
		// free(node->command->outfile);
		// return (1);
		// printf("invalid outfile = 1\n");
		new->invalid = 1;
	}
	return (0);
}

int	handle_heredoc_parse(t_parser *p, t_cmdlist *node)
{
	p->current = p->current->next;
	if (!p->current || p->current->type != TOKEN_WORD)
		return (set_error(p, "syntax error near unexpected token `newline'", 2));
	if (node->command->here_doc != 0)
		free(node->command->limiter);
	// if (!*p->current->value)
	// 	node->command->limiter = ft_strdup("");
	// else if ()
	node->command->limiter = ft_strdup(p->current->value);
	if (!node->command->limiter)
		return (1);
	node->command->here_doc = 1;
	return (0);
}

int		process_token(t_parser *p, t_cmdlist *node)
{
	int err;

	err = 0;
	if (p->current->type == TOKEN_REDIR_IN)
		err = handle_infile(p, node);
	else if (p->current->type == TOKEN_REDIR_OUT)
		err = handle_outfile(p, node, 0);
	else if (p->current->type == TOKEN_REDIR_APPEND)
		err = handle_outfile(p, node, 1);
	else if (p->current->type == TOKEN_REDIR_HEREDOC)
		err = handle_heredoc_parse(p, node);
	return (err);
}