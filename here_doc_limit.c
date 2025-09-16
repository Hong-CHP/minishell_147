#include "minishell.h"
#include "libft.h"

void	if_quote_in_limit_heredoc(t_hd_limit *limit, char *input, int *i)
{
	char	quote;

	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
		limit->word[limit->len++] = input[(*i)++];
	if (input[*i] == quote)
		(*i)++;
}

int    init_here_doc(t_hd_limit *limit, char *input)
{
    limit->word = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!limit->word)
		return (0);
	ft_memset(limit->word, 0, ft_strlen(input) + 1);
    limit->len = 0;
    limit->dollar = 0;
    return (1);
}

void    if_dollar_in_limit_heredoc(t_hd_limit *limit, char *input, int *i)
{
	if ((input[*i + 1] && (input[*i + 1] == '\'' || input[*i + 1] == '"')))
	{
		(*i)++;
		if_quote_in_limit_heredoc(limit, input, i);
	}
	else if (input[*i + 1] && input[*i + 1] == '$')
	{
		limit->word[limit->len++] = input[(*i)++];
		limit->dollar++;
		if (input[*i] == '$' && (input[*i + 1] == '\'' || input[*i + 1] == '"') && limit->dollar % 2 == 1)
			limit->word[limit->len++] = '$';
	}
	else
		limit->word[limit->len++] = input[(*i)++];
}

char	*get_heredoc_limit(char *input, int *move)
{
    t_hd_limit  limit;
	int		i;

	i = 0;
    if (!init_here_doc(&limit, input))
    {
        return (NULL);
    }
	while (input[i] && input[i] != ' ')
	{
		if (input[i] == '$')
            if_dollar_in_limit_heredoc(&limit, input, &i);
		else if (input[i] == '"' || input[i] == '\'')
			if_quote_in_limit_heredoc(&limit, input, &i);
		else
			limit.word[limit.len++] = input[i++];
	}
	*move = i;
	limit.word[limit.len++] = '\0';
	return (limit.word);
}
