#include "minishell.h"
#include "libft.h"

void	extract_word_front(t_handler_qt *cur, char **res, t_parser *parser, t_varlist **head_var)
{
	char			*tmp;
	char			*new;

	if (parser->input[cur->start_qt_input] == '\'')
	{
		new = ft_strjoin(*res, cur->part);
		free(*res);
		*res = new;		
	}
	else if (parser->input[cur->start_qt_input] == '"')
	{
		tmp = reg_dollar_sign(cur->part, head_var, parser);
		new = ft_strjoin(*res, tmp);
		free(*res);
		*res = new;
		free(tmp);
	}
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

char	*init_buf_for_extract(char **buf, t_parser *parser, t_handler_qt **handler)
{
	*buf = malloc(sizeof(char) * (ft_strlen(parser->input) + 1));
	if (!*buf)
		return (NULL);
	ft_memset(*buf, 0, sizeof(char) * (ft_strlen(parser->input) + 1));
	if (!extract_by_type_sign(*buf, parser, handler))
	{
		clean_handler_and_buf_for_extract(*buf, handler);
		return (NULL);
	}
	return (*buf);
}

void 	handler_dollar_in_end(char *str, char **res, t_varlist **head_var, t_parser *parser)
{
	char	*part;

	part = ft_strdup(str);
	handler_dollar_in_word(part, res, head_var, parser);
}

char *extract_word(t_parser *parser, t_varlist **head_var, int *last_pos, char **res)
{
	char			*buf;
	t_handler_qt	*handler;
	t_handler_qt	*cur;
	char			*part;

	handler = NULL;
	if (!init_buf_for_extract(&buf, parser, &handler))
		return (NULL);
	cur = handler;
	while (cur)
	{
		if (cur->start_qt_buf > *last_pos)
		{
			part = ft_substr(buf, *last_pos, cur->start_qt_buf - *last_pos);
			handler_dollar_in_word(part, res, head_var, parser);
		}
		extract_word_front(cur, res, parser, head_var);
		*last_pos = cur->end_qt_buf + 1;
		cur = cur->next;
	}
	if (*last_pos < (int)(ft_strlen(buf)))
		handler_dollar_in_end(&buf[*last_pos], res, head_var, parser);
	clean_handler_and_buf_for_extract(buf, &handler);
	return (*res);
}
