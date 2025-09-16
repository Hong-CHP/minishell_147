#include "minishell.h"
#include "libft.h"

t_token	*create_pipe_token(t_parser *parser)
{
	t_token	*tok;
	char	*val;

	if (parser->input[parser->pos]
		&& parser->input[parser->pos + 1] == '|')
	{
		val = ft_strdup("||");
		parser->pos += 2;
		tok = create_token(TOKEN_OR, val);
	}
	else if (parser->input[parser->pos]
			&& parser->input[parser->pos + 1] != '|')
	{
		val = ft_strdup("|");
		parser->pos++;
		tok = create_token(TOKEN_PIPE, val);
	}
	free(val);
	if (!val || !tok)
		return (NULL);
	return (tok);
}

t_token	*create_input_token(t_parser *parser)
{
	t_token	*tok;
	char	*val;

	if (parser->input[parser->pos]
		&& parser->input[parser->pos + 1] == '<')
	{
		val = ft_strdup("<<");
		parser->pos += 2;
		tok = create_token(TOKEN_REDIR_HEREDOC, val);
	}
	else if (parser->input[parser->pos]
			&& parser->input[parser->pos + 1] != '<')
	{
		val = ft_strdup("<");
		parser->pos++;
		tok = create_token(TOKEN_REDIR_IN, val);
	}
	free(val);
	if (!val || !tok)
		return (NULL);
	return (tok);
}

t_token	*create_output_token(t_parser *parser)
{
	t_token	*tok;
	char	*val;

	if (parser->input[parser->pos]
		&& parser->input[parser->pos + 1] == '>')
	{
		val = ft_strdup(">>");
		parser->pos += 2;
		tok = create_token(TOKEN_REDIR_APPEND, val);
	}
	else if (parser->input[parser->pos]
			&& parser->input[parser->pos + 1] != '>')
	{
		val = ft_strdup(">");
		parser->pos++;
		tok = create_token(TOKEN_REDIR_OUT, val);
	}
	free(val);
	if (!val || !tok)
		return (NULL);
	return (tok);
}

t_token	*create_and_token(t_parser *parser)
{
	t_token *tok;
	char    *val;

	if (parser->input[parser->pos]
		&&parser->input[parser->pos + 1] == '&')
	{
		val = ft_strdup("&&");
		parser->pos += 2;
		tok = create_token(TOKEN_AND, val);
		free(val);
		if (!val || !tok)
			return (NULL);
		return (tok);
	}
	set_error(parser, "Unexpected character '&'", 2);
	return (NULL);
}

t_token	*create_simple_token(t_parser *parser, int type, const char *s)
{
	t_token	*tok;
	char	*val;

	val = ft_strdup(s);
	if (!val)
		return (NULL);
	tok = create_token(type, val);
	free(val);
	if (!tok)
		return (NULL);
	else
		parser->pos++;
	return (tok);
}