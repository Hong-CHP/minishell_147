#include "minishell.h"
#include "libft.h"

char	*extract_if_slash(char *buf, t_parser *parser, int *len)
{
	parser->pos++;

	if (!parser->input[parser->pos])
	{
		set_error(parser, "incorrect syntaxe", 1);
		return (NULL);
	}
	if (parser->input[parser->pos])
		buf[(*len)++] = parser->input[parser->pos++];
	return (buf);
}

char	*extract_by_slash_quotes(t_parser *parser, int *len, char *buf)
{
	if (parser->input[parser->pos] == '$' && (parser->input[parser->pos + 1] == '"'
		|| parser->input[parser->pos + 1] == '\''))
	{
		parser->pos++;
		while (parser->input[parser->pos] && parser->input[parser->pos] != '"'
				&& parser->input[parser->pos] != '\'')
			buf[(*len)++] = parser->input[parser->pos++];
	}
	return (buf);
}

int		extract_loop_body(char *buf, t_parser *parser, t_handler_qt **handler, int *len)
{
	if (parser->input[parser->pos] == '\\')
	{
		if(!extract_if_slash(buf, parser, len))
			return (0);
	}
	else if (parser->input[parser->pos] == '\'')
	{
		if(!extract_if_single_quote(buf, parser, len, handler))
			return (0);
	}
	else if (parser->input[parser->pos] == '"')
	{
		if (!extract_if_double_quote(buf, parser, len, handler))
			return (0);
	}
	else
	{
		buf[(*len)++] = parser->input[parser->pos++];
		return (1);
	}
	return (1);
}

char	*extract_by_type_sign(char *buf, t_parser *parser, t_handler_qt **handler)
{
	int		len;

	len = 0;
	while (parser->input[parser->pos] && !is_separator(parser->input[parser->pos]))
	{
		extract_by_slash_quotes(parser, &len, buf);
	    if (!extract_loop_body(buf, parser, handler, &len))
            return (NULL);
	}
	buf[len] = '\0';
	return (buf);
}

// node->start_qt_input = parser->pos;
// node->start_qt_buf = *len;
// if (parser->input[parser->pos] == '\'')
// {
// 	node->part = ft_substr(parser->input, node->start_qt_input + 1, node->end_qt_input - node->start_qt_input - 1);
// 	if (!node->part)
// 		node->part = ft_strdup("\n");
// 	parser->pos++;
// 	add_handler_lst_back(handler, node);
// 	return (buf);
// }
// else
// {
// 	free(node);
// 	set_error(parser, "unclosed quote", 1);
// 	return (NULL);
// }
// node->start_qt_input = parser->pos;
// node->start_qt_buf = *len;
// if (parser->input[parser->pos] == '"')
// {
// 	node->part = ft_substr(parser->input, node->start_qt_input + 1, node->end_qt_input - node->start_qt_input - 1);
// 	if (!node->part)
// 		node->part = ft_strdup("\n");
// 	parser->pos++;
// 	add_handler_lst_back(handler, node);
// 	return (buf);
// }
// else if (!parser->input[parser->pos])
// {
// 	free(node);
// 	set_error(parser, "incorrect syntaxe", 1);
// 	return (NULL);
// }