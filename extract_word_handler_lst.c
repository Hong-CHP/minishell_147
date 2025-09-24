#include "minishell.h"
#include "libft.h"

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

char	*extract_if_single_quote(char *buf, t_parser *parser, int *len, t_handler_qt **handler)
{
	t_handler_qt	*node;

	node = new_handler_node(parser->pos, *len);
	if (!node)
		return (NULL);
	parser->pos++;
	while (parser->input[parser->pos]
		&& parser->input[parser->pos] != '\'')
		buf[(*len)++] = parser->input[parser->pos++];
	node->end_qt_buf = (*len) - 1;
	return (handler_end_single_quote_data(parser, node, handler, buf));
}

char	*extract_if_double_quote(char *buf, t_parser *parser, int *len, t_handler_qt **handler)
{
	t_handler_qt	*node;

	node = new_handler_node(parser->pos, *len);
	if (!node)
		return (NULL);
	parser->pos++;
	while (parser->input[parser->pos] && parser->input[parser->pos] != '"')
	{
		if (parser->input[parser->pos] == '\\' && (parser->input[parser->pos + 1] == '"'
			|| parser->input[parser->pos + 1] == '\\' || parser->input[parser->pos + 1] == '$'))
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
	node->end_qt_buf = (*len) - 1;
	if (!handler_end_double_quote_data(parser, node, handler, buf))
		return (NULL);
	return (buf);
}