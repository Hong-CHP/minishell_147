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

char	*extract_by_type_sign(char *buf, t_parser *parser, t_handler_qt **handler)
{
	int		len;

	len = 0;
	while (parser->input[parser->pos] && !is_separator(parser->input[parser->pos]))
	{
		if (parser->input[parser->pos] == '$' && (parser->input[parser->pos + 1] == '"' || parser->input[parser->pos + 1] == '\''))
		{
			parser->pos++;
			while (parser->input[parser->pos] && parser->input[parser->pos] != '"' && parser->input[parser->pos] != '\'')
				buf[len++] = parser->input[parser->pos++];
		}
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

// char *extract_word(t_parser *parser, t_varlist **head_var)
// {
// 	char			*buf;
// 	char			*tmp;
// 	t_handler_qt	*handler;
// 	t_handler_qt	*cur;
// 	int				last_pos;
// 	char			*part;
// 	(void)head_var;
// 	char			*res;

// 	buf = malloc(sizeof(char) * (ft_strlen(parser->input) + 1));
// 	if (!buf)
// 		return (NULL);
// 	ft_memset(buf, 0, sizeof(char) * (ft_strlen(parser->input) + 1));
// 	handler = NULL;
// 	if (!extract_by_type_sign(buf, parser, &handler))
// 		return (NULL);
// 	// printf("buf in extract word before handle is :%s\n", buf);
// 	cur = handler;
// 	last_pos = 0;
// 	res = ft_strdup("");
// 	while (cur)
// 	{
// 		if (cur->start_qt_buf > last_pos)
// 		{
// 			part = ft_substr(buf, last_pos, cur->start_qt_buf - last_pos);
// 			if (ft_strchr(part, '$'))
// 			{
// 				tmp = part;
// 				part = reg_dollar_sign(part, head_var);
// 				free(tmp);
// 			}
// 			// printf("part is: %s\n", part);
// 			res = ft_strjoin(res, part);
// 			free(part);
// 		}
// 		if (parser->input[cur->start_qt_input] == '\'')
// 			res = ft_strjoin(res, cur->part);
// 		else if (parser->input[cur->start_qt_input] == '"')
// 		{
// 			tmp = reg_dollar_sign(cur->part, head_var);
// 			res = ft_strjoin(res, tmp);
// 			free(tmp);
// 		}
// 		// printf("res is: %s\n", res);
// 		last_pos = cur->end_qt_buf + 1;
// 		// printf("joined res string is: %s///////and last pos is %d\n", res, last_pos);
// 		cur = cur->next;
// 	}
// 	if (last_pos < (int)(ft_strlen(buf)))
// 	{
// 		part = ft_strdup(&buf[last_pos]);
// 		// printf("part is: %s\n", part);
// 		if (ft_strchr(&buf[last_pos], '$'))
// 		{
// 			tmp = part;
// 			part = reg_dollar_sign(tmp, head_var);
// 			free(tmp);
// 		}
// 		// printf("part is: %s\n", part);
// 		res = ft_strjoin(res, part);
// 		free(part);
// 	}
// 	// printf("final res string is: %s\n", res);
// 	free(buf);
// 	free_handler_lst(&handler);
// 	return (res);
// }

char *extract_word(t_parser *parser, t_varlist **head_var)
{
	char			*buf;
	char			*tmp;
	t_handler_qt	*handler;
	t_handler_qt	*cur;
	int				last_pos;
	char			*part;
	(void)head_var;
	char			*res;
	char			*new;

	buf = malloc(sizeof(char) * (ft_strlen(parser->input) + 1));
	if (!buf)
		return (NULL);
	ft_memset(buf, 0, sizeof(char) * (ft_strlen(parser->input) + 1));
	handler = NULL;
	if (!extract_by_type_sign(buf, parser, &handler))
		return (NULL);
	// printf("buf in extract word before handle is :%s\n", buf);
	cur = handler;
	last_pos = 0;
	res = ft_strdup("");
	while (cur)
	{
		if (cur->start_qt_buf > last_pos)
		{
			part = ft_substr(buf, last_pos, cur->start_qt_buf - last_pos);
			if (ft_strchr(part, '$'))
			{
				tmp = part;
				part = reg_dollar_sign(part, head_var);
				free(tmp);
			}
			// printf("part is: %s\n", part);
			new = ft_strjoin(res, part);
			free(res);
			res = new;
			free(part);
		}
		if (parser->input[cur->start_qt_input] == '\'')
		{
			new = ft_strjoin(res, cur->part);
			free(res);
			res = new;
		}
		else if (parser->input[cur->start_qt_input] == '"')
		{
			tmp = reg_dollar_sign(cur->part, head_var);
			new = ft_strjoin(res, tmp);
			free(tmp);
			free(res);
			res = new;
		}
		// printf("res is: %s\n", res);
		last_pos = cur->end_qt_buf + 1;
		// printf("joined res string is: %s///////and last pos is %d\n", res, last_pos);
		cur = cur->next;
	}
	if (last_pos < (int)(ft_strlen(buf)))
	{
		// printf("$/ entre here\n");
		part = ft_strdup(&buf[last_pos]);
		// printf("part is: %s\n", part);
		if (ft_strchr(&buf[last_pos], '$'))
		{
			tmp = part;
			part = reg_dollar_sign(tmp, head_var);
			free(tmp);
		}
		// printf("part is: %s\n", part);
		new = ft_strjoin(res, part);
		free(res);
		res = new;
		free(part);
	}
	// printf("final res string is: %s\n", res);
	free(buf);
	free_handler_lst(&handler);
	return (res);
}