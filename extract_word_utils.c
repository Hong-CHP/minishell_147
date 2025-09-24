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

void	clean_handler_and_buf_for_extract(char *buf, t_handler_qt **handler)
{
	if (buf)
		free(buf);
	if (handler && *handler)
		free_handler_lst(handler);
}

int is_separator(char c)
{
    return (c == ' '  || c == '\t' || c == '\n' ||
            c == '|'  || c == '<'  || c == '>'  ||
            c == '&'  || c == ';'  ||
            c == '('  || c == ')');
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