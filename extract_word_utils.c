#include "minishell.h"
#include "libft.h"

char	*handler_end_single_quote_data(t_parser *parser, t_handler_qt *node, t_handler_qt **handler, char *buf)
{
	node->end_qt_input = parser->pos;
	if (parser->input[parser->pos] == '\'' || parser->input[parser->pos] == '"')
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

char	*handler_end_double_quote_data(t_parser *parser, t_handler_qt *node, t_handler_qt **handler, char *buf)
{
	node->end_qt_input = parser->pos;
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

t_handler_qt	*new_handler_node(int start, int pos_len)
{
	t_handler_qt	*node;

	node = malloc(sizeof(t_handler_qt));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_handler_qt));
	node->start_qt_input = start;
	node->end_qt_input = 0;
	node->start_qt_buf = pos_len;
	node->end_qt_buf = 0;
	node->dollar = 0;
	node->part = NULL;
	node->next = NULL;
	return (node);
}

void	add_handler_lst_back(t_handler_qt **handler, t_handler_qt *node)
{
	t_handler_qt	*cur;

	if (!*handler)
		*handler = node;
	else
	{
		cur = *handler;
		while (cur->next)
			cur = cur->next;
		cur->next = node;
	}
}

void	free_handler_lst(t_handler_qt **handler)
{
	t_handler_qt *cur;
	t_handler_qt *next;

	cur = *handler;
	while (cur)
	{
		next = cur->next;
		if (cur->part)
			free(cur->part);
		free(cur);
		cur = next;
	}
	handler = NULL;
}

void	handler_dollar_in_word(char *part, char **res, t_varlist **head_var, t_parser *parser)
{
	char	*tmp;
	char	*new;

	if (ft_strchr(part, '$'))
	{
		tmp = part;
		part = reg_dollar_sign(tmp, head_var, parser);
		free(tmp);
	}
	new = ft_strjoin(*res, part);
	free(*res);
	*res = new;
	free(part);
}

void	join_free_for_word(char **res, char *str)
{
	char	*new;

	new = ft_strjoin(*res, str);
	free(*res);
	*res = new;
} 

char	*init_buf_for_extract(char **buf, t_parser *parser, t_handler_qt **handler)
{
	*buf = malloc(sizeof(char) * (ft_strlen(parser->input) + 1));
	if (!*buf)
		return (NULL);
	ft_memset(*buf, 0, sizeof(char) * (ft_strlen(parser->input) + 1));
	if (!extract_by_type_sign(*buf, parser, handler))
	{
		free(*buf);
		return (NULL);
	}
	return (*buf);
}

void	extract_word_front(t_handler_qt *cur, char **res, t_parser *parser, t_varlist **head_var)
{
	char			*tmp;

	if (parser->input[cur->start_qt_input] == '\'')
		join_free_for_word(res, cur->part);
	else if (parser->input[cur->start_qt_input] == '"')
	{
		tmp = reg_dollar_sign(cur->part, head_var, parser);
		join_free_for_word(res, tmp);
		free(tmp);
	}
}
