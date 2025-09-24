#include "minishell.h"
#include "libft.h"

int	is_varname_format_brackets(char *str, int i)
{
	i++;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != '}')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '}')
		return (i);
	else
		return (0);
}

int	is_varname_format(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '{')
		return (is_varname_format_brackets(str, i) - 1);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != ' ' && str[i] != ',' && str[i] != '$'
		&& str[i] != '\\' && str[i] != '\'' && str[i] != '"')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (i);
		i++;
	}
	return (i);
}

void	handle_dollar_sequence(char *str, char **vars, char **vals, t_fill *ctx)
{
	int	o;

	o = 0;
	ctx->i++;
	if (str[ctx->i] == '{')
		ctx->i++;
	if (vars[ctx->k])
	{
		while (vals[ctx->k][o])
			ctx->word[ctx->j++] = vals[ctx->k][o++];
		ctx->i += ft_strlen(vars[ctx->k]);
		if (str[ctx->i] == '}')
		{
			ctx->word[ctx->j++] = '}';
			ctx->i++;
		}
		ctx->k++;
	}
	else
		ctx->word[ctx->j++] = '$';
}

char	*fill_words_with_real_vals(char *str, char **vars, char **vals, int len)
{
	t_fill	ctx;
	char	*word;

	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	ctx.i = 0;
	ctx.j = 0;
	ctx.k = 0;
	ctx.word = word;
	while (ctx.j < len)
	{
		if (str[ctx.i] == '$' && str[ctx.i + 1] && str[ctx.i + 1] != '$')
			handle_dollar_sequence(str, vars, vals, &ctx);
		else
			word[ctx.j++] = str[ctx.i++];
	}
	word[ctx.j] = '\0';
	return (ctx.word);
}

char	*replace_by_real_val(t_varlist **head, t_parser *p, int nb_v, char *str)
{
	char	*word;
	int		t_len;
	char	**vars;
	char	**vals;

	vars = malloc(sizeof(char *) * (nb_v + 1));
	if (!vars)
		return (NULL);
	find_dollar_sign(str, vars);
	vals = malloc(sizeof(char *) * (nb_v + 1));
	if (!vals)
	{
		free_vars_vals(vars, vals);
		return (NULL);
	}
	t_len = get_vals_and_tot_len(str, vals, vars, head, p);
	word = fill_words_with_real_vals(str, vars, vals, t_len);
	if (!word)
	{
		free_vars_vals(vars, vals);
		return (NULL);
	}
	free_vars_vals(vars, vals);
	return (word);
}
