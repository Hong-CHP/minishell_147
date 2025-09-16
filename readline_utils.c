#include "minishell.h"
#include "libft.h"

int	has_unclosed_quote(char *input)
{
	int		i;
	char	quote;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (input[i] == quote)
				i++;
			else
				return (1);
		}
		else
			i++;
	}
	return (0);
}
