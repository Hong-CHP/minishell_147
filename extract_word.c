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

char	*extract_if_single_quote(char *buf, t_parser *parser, int *len, t_handler_qt **handler)
{
	t_handler_qt	*node;

	node = new_handler_node(parser->pos);
	if (!node)
		return (NULL);
	node->start_qt_input = parser->pos;
	node->start_qt_buf = *len;
	parser->pos++;
	while (parser->input[parser->pos]
		&& parser->input[parser->pos] != '\'')
		buf[(*len)++] = parser->input[parser->pos++];
	node->end_qt_input = parser->pos;
	node->end_qt_buf = (*len) - 1;
	if (parser->input[parser->pos] == '\'')
	{
		node->part = ft_substr(parser->input, node->start_qt_input + 1, node->end_qt_input - node->start_qt_input - 1);
		if (!node->part)
			node->part = ft_strdup("\n");
		parser->pos++;
		add_handler_lst_back(handler, node);
		return (buf);
	}
	else
	{
		free(node);
		set_error(parser, "unclosed quote", 1);
		return (NULL);
	}
}

char	*extract_if_double_quote(char *buf, t_parser *parser, int *len, t_handler_qt **handler)
{
	t_handler_qt	*node;

	node = new_handler_node(parser->pos);
	if (!node)
		return (NULL);
	node->start_qt_input = parser->pos;
	node->start_qt_buf = *len;
	parser->pos++;
	while (parser->input[parser->pos]
		&& parser->input[parser->pos] != '"')
	{
		if (parser->input[parser->pos] == '\\'
			&& (parser->input[parser->pos + 1] == '"'
			|| parser->input[parser->pos + 1] == '\\'
			|| parser->input[parser->pos + 1] == '$'))
		{
			parser->pos++;
			buf[(*len)++] = parser->input[parser->pos++];
		}
		else
		{
			if (parser->input[parser->pos] == '$')
				node->dollar++;
			buf[(*len)++] = parser->input[parser->pos++];
		}
	}
	node->end_qt_input = parser->pos;
	node->end_qt_buf = (*len) - 1;
	if (parser->input[parser->pos] == '"')
	{
		node->part = ft_substr(parser->input, node->start_qt_input + 1, node->end_qt_input - node->start_qt_input - 1);
		if (!node->part)
			node->part = ft_strdup("\n");
		parser->pos++;
		add_handler_lst_back(handler, node);
		return (buf);
	}
	else if (!parser->input[parser->pos])
	{
		free(node);
		set_error(parser, "incorrect syntaxe", 1);
		return (NULL);
	}
	return (buf);
}

int is_separator(char c)
{
    return (c == ' '  || c == '\t' || c == '\n' ||
            c == '|'  || c == '<'  || c == '>'  ||
            c == '&'  || c == ';'  ||
            c == '('  || c == ')');
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

char	*extract_by_type_sign(char *buf, t_parser *parser, t_handler_qt **handler)
{
	int		len;

	len = 0;
	while (parser->input[parser->pos] && !is_separator(parser->input[parser->pos]))
	{
		// extract_by_slash_quotes
		if (parser->input[parser->pos] == '\\')
		{
			if(!extract_if_slash(buf, parser, &len))
				return (NULL);
		}
		else if (parser->input[parser->pos] == '\'')
		{
			if(!extract_if_single_quote(buf, parser, &len, handler))
				return (NULL);
		}
		else if (parser->input[parser->pos] == '"')
		{
			if (!extract_if_double_quote(buf, parser, &len, handler))
				return (NULL);
		}
		else
			buf[len++] = parser->input[parser->pos++];
	}
	buf[len] = '\0';
	return (buf);
}

int	if_still_space(char *res)
{
	int	i;

	i = 0;
	while (res[i])
	{
		if (!ft_isalnum(res[i]) && res[i] != ' ')
			return (0);
		else if (res[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

char *extract_word(t_parser *parser, t_varlist **head_var)
{
	char			*buf;
	// char			*tmp;
	t_handler_qt	*handler;
	t_handler_qt	*cur;
	int				last_pos;
	char			*part;
	(void)head_var;
	char			*res;
	// char			*new;

	handler = NULL;
	if (!init_buf_for_extract(&buf, parser, &handler))
		return (NULL);
	last_pos = 0;
	res = ft_strdup("");
	cur = handler;
	while (cur)
	{
		if (cur->start_qt_buf > last_pos)
		{
			part = ft_substr(buf, last_pos, cur->start_qt_buf - last_pos);
			handler_dollar_in_word(part, &res, head_var);
		}
		extract_word_front(cur, &res, parser, head_var);
		last_pos = cur->end_qt_buf + 1;
		cur = cur->next;
	}
	if (last_pos < (int)(ft_strlen(buf)))
	{
		part = ft_strdup(&buf[last_pos]);
		handler_dollar_in_word(part, &res, head_var);
	}
	free(buf);
	free_handler_lst(&handler);
	return (res);
}
