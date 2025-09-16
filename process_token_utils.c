#include "minishell.h"
#include "libft.h"

void	ft_put_err_msg(t_parser *parser, char *str, char *file_name)
{
	if (parser->error && parser->error_msg)
	{
		ft_putstr_fd("minishell: ", 2);
		if (file_name && *file_name)
		{
			ft_putstr_fd(file_name, 2);
			ft_putstr_fd(": ", 2);
		}
		ft_putstr_fd(parser->error_msg, 2);
		if (str && *str)
		{
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(str, 2);
		}
		ft_putstr_fd("\n", 2);
	}
}

int	set_error(t_parser *p, char *msg, int err_code)
{
	if (p->error_msg)
		free(p->error_msg);
	p->error = 1;
	p->error_msg = ft_strdup(msg);
	g_exit_status = err_code;
	return (1);
}

int	is_cmd_token(int type)
{
	return (type == TOKEN_WORD
		 || type == TOKEN_REDIR_IN
		 || type == TOKEN_REDIR_OUT
		 || type == TOKEN_REDIR_APPEND
		 || type == TOKEN_REDIR_HEREDOC);
}

int	invalide_syntaxe_token(t_parser *parser, t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (!cur)
		return (0);
	if (cur->type == TOKEN_PIPE)
	{
		set_error(parser, "syntax error near unexpected token '|'", 2);
		return (1);
	}
	while (cur)
	{
		if (cur->type == TOKEN_PIPE && (!cur->next->value || cur->next->type == TOKEN_PIPE))
		{
			set_error(parser, "syntax error near unexpected token '|'", 2);
			return (1);
		}
		if (cur->type == TOKEN_OR && cur->next->type == TOKEN_PIPE)
		{
			set_error(parser, "syntax error near unexpected token '|'", 2);
			return (1);
		}
		if (cur->type == TOKEN_LPAREN || cur->type == TOKEN_RPAREN
			|| cur->type == TOKEN_SEMICOLON)
		{
			set_error(parser, "syntax error", 2);
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}
