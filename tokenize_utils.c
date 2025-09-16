#include "minishell.h"
#include "libft.h"

int		if_quote(char *str)
{
	int		i;
	char	ch;

	i = 0;
	while (str[i] && str[i] != '\''&& str[i] != '"')
		i++;
	if (str[i] == '\'' || str[i] == '"')
	{
		ch = str[i];
		i++;
	}
	else
		return (0);
	while (str[i] && str[i] != ch)
		i++;
	if (str[i] == ch)
		return ((int)ch);
	if (!str[i])
		return (-1);
	return (0);
}

void	free_token_list(t_token **head)
{
	t_token	*cur;
	t_token	*next;

	cur = *head;
	while (cur && cur->next)
	{
		next = cur->next;
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

char	*extract_exit_code(char *str)
{
	char	*exit_code;
	int		first_len;
	char	*first;
	char	*tmp;
	int		last_len;
	char	*last;
	
	exit_code = ft_strdup("");
	first_len = ft_strnstr(str, "$?", ft_strlen(str)) - str;
	first = ft_substr(str, 0, first_len);
	exit_code = ft_strjoin(exit_code, first);
	tmp = ft_itoa(g_exit_status);
	exit_code = ft_strjoin(exit_code, tmp);
	last_len = ft_strlen(str) - first_len + 2;
	last = ft_substr(str, first_len + 2, last_len);
	exit_code = ft_strjoin(exit_code, last);
	free(str);
	str = exit_code;
	free(first);
	free(last);
	return (str);
}

char	*extract_value_if_sign(char *value, t_varlist **head_var)
{
	char	*buf;
	int		i;
	int		j;
	int		len;
	int		state;
	char	*tmp;
	char	*expanded;
	(void)head_var;

	buf = malloc(sizeof(char) * (ft_strlen(value) * 2 + 1));
	if (!buf)
		return (NULL);
	len = 0;
	i = 0;
	state = 0;
	while (value[i])
	{
		if (state == 0)
		{
			if (value[i] == '\'')
			{
				state = 1;
				i++;
			}
			else if (value[i] == '\"')
			{
				state = 2;
				i++;
			}
			else if (value[i] == '$')
			{
				int k = i;
				j = 0;
				tmp = malloc(sizeof(char) * (ft_strlen(value) * 2 + 1));
				if (!tmp)
					return (NULL);
				while (value[k] && value[k] != '\'' && value[k] != '"')
					tmp[j++] = value[k++];
				tmp[j] = '\0';
				expanded = reg_dollar_sign(tmp, head_var);
				if (!expanded)
					expanded = ft_strdup("");
				ft_memcpy(buf + len, expanded, ft_strlen(expanded));
				len += ft_strlen(expanded);
				i += ft_strlen(tmp);
				free(expanded);
				free(tmp);
			}
			else if (value[i] == '\\')
			{
				i++;
				if (value[i])
					buf[len++] = value[i++];
				else
				{
					printf("incorrect syntaxe\n");
					free(buf);
					return (NULL);
				}
			}
			else
				buf[len++] = value[i++];
		}
		else if (state == 1)
		{
			if (value[i] == '\'')
			{
				state = 0;
				i++;
			}
			else
				buf[len++] = value[i++];
		}
		else if (state == 2)
		{
			if (value[i] == '"')
			{
				state = 0;
				i++;
			}
			else if (value[i] == '\\' &&
						(value[i + 1] == '\\' || value[i + 1] == '$' || value[i + 1] == '\"'))
			{
				i++;
				buf[len++] = value[i++];
			}
			else if (value[i] == '$')
			{
				int k = i;
				j = 0;
				tmp = malloc(sizeof(char) * (ft_strlen(value) * 2 + 1));
				if (!tmp)
					return (NULL);
				while (value[k] && value[k] != '\'' && value[k] != '"')
					tmp[j++] = value[k++];
				tmp[j] = '\0';
				expanded = reg_dollar_sign(tmp, head_var);
				if (!expanded)
					expanded = ft_strdup("");
				ft_memcpy(buf + len, expanded, ft_strlen(expanded));
				len += ft_strlen(expanded);
				i += ft_strlen(tmp);
				// while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
				// 	i++;
				free(expanded);
				free(tmp);
			}
			else
				buf[len++] = value[i++];
		}
	}
	buf[len] = '\0';
	return (buf);
}

// char	*extract_value_if_sign(char *value, t_varlist **head_var)
// {
// 	char	*buf;
// 	int		i;
// 	int		len;
// 	int		dollar;
// 	int		start;
// 	int		end;
// 	char 	*part;
// 	char	*pos;
// 	int		len_part;
// 	char	*front;
// 	char	*tmp;
// 	char	*joined;

// 	buf = malloc(sizeof(char) * (ft_strlen(value) + 1));
// 	if (!buf)
// 		return (NULL);
// 	len = 0;
// 	start = 0;
// 	end = 0;
// 	i = 0;
// 	dollar = 0;
// 	while (value[i])
// 	{
// 		if (value[i] == '\\')
// 		{
// 			i++;
// 			if (!value[i])
// 			{
// 				printf("incorrect syntaxe\n");
// 				return (NULL);
// 			}
// 			else
// 				buf[len++] = value[i++];
// 		}
// 		else if (value[i] == '\'')
// 		{
// 			i++;
// 			start = i;
// 			while (value[i] && value[i] != '\'')
// 			{
// 				if (value[i] == '$')
// 					dollar++;
// 				buf[len++] = value[i++];
// 			}
// 			if (value[i] == '\'')
// 			{
// 				i++;
// 				end = i - 1;
// 			}
// 			else
// 			{
// 				printf("incorrect syntaxe\n");
// 				return (NULL);
// 			}
// 		}
// 		else if (value[i] == '"')
// 		{
// 			i++;
// 			while (value[i] && value[i] != '"')
// 			{
// 				if (value[i] == '\\' && (value[i + 1] == '"'
// 					|| value[i + 1] == '$' || value[i + 1] == '\\'))
// 					i++;
// 				buf[len++] = value[i++];
// 			}
// 			if (value[i] == '"')
// 				i++;
// 			else if (!value[i])
// 			{
// 				printf("here print: incorrect syntaxe\n");
// 				return (NULL);
// 			}
// 		}
// 		else
// 			buf[len++] = value[i++];
// 	}
// 	buf[len] = '\0';
// 	if (start > 0 && end > 0 && dollar > 0)
// 	{
// 		part = ft_substr(buf, start, end - start);
// 		if (!part)
// 			return (NULL);
// 		len_part = ft_strlen(part);
// 		pos = ft_strnstr(buf, part, ft_strlen(buf));
// 		front = ft_substr(buf, 0, pos - buf);
// 		if (ft_strchr(front, '$'))
// 		{
// 			tmp = front;
// 			front = reg_dollar_sign(front, head_var);
// 			free(tmp);
// 		}
// 		joined = ft_strjoin(front, part);
// 		free(front);
// 		free(part);
// 		if (*(pos + len_part))
// 		{
// 			part = ft_substr(pos, len_part, ft_strlen(pos) - len_part);
// 			if (ft_strchr(part, '$'))
// 			{
// 				tmp = part;
// 				part = reg_dollar_sign(tmp, head_var);
// 				free(tmp);
// 			}
// 			tmp = joined;
// 			joined = ft_strjoin(tmp, part);
// 			free(tmp);
// 			free(part);
// 		}
// 		free(buf);
// 		buf = joined;
// 	}
// 	else if (ft_strchr(buf, '$') && dollar == 0)
// 	{
// 		if (ft_strnstr(buf, "$?", ft_strlen(buf)))
// 			extract_exit_code(buf);
// 		else
// 		{
// 			tmp = buf;
// 			buf = reg_dollar_sign(buf, head_var);
// 			free(tmp);
// 		} 
// 	}
// 	return (buf);
// }

// 		if (value[i] == '\\')
// 		{
// 			start[j++] = i;
// 			i++;
// 			if (!value[i])
// 			{
// 				printf("incorrect syntaxe\n");
// 				return (NULL);
// 			}
// 			else
// 				buf[len++] = value[i++];
// 		}
// 		else if (value[i] == '\'')
// 		{
// 			start[j++] = i;
// 			i++;
// 			while (value[i] && value[i] != '\'')
// 				buf[len++] = value[i++];
// 			if (value[i] == '\'')
// 				i++;
// 			else
// 			{
// 				printf("incorrect syntaxe\n");
// 				return (NULL);
// 			}
// 		}
// 		else if (value[i] == '"')
// 		{
// 			start[j++] = i;
// 			i++;
// 			while (value[i] && value[i] != '"')
// 			{
// 				if (value[i] == '\\' && (value[i + 1] == '"'
// 					|| value[i + 1] == '$' || value[i + 1] == '\\'))
// 				{
// 					i++;
// 				}
// 				else if (value[i] == '$')
// 					dollar_pos[dollar++] = i;
// 				buf[len++] = value[i++];
// 			}
// 			if (value[i] == '"')
// 				i++;
// 			else if (!value[i])
// 			{
// 				printf("here print: incorrect syntaxe\n");
// 				return (NULL);
// 			}
// 		}
// 		else
// 		{
// 			if (value[i] == '$')
// 				dollar_pos[dollar++] = i;
// 			buf[len++] = value[i++];
// 		}
// 	}
// 	buf[len] = '\0';
// 	printf("buf is %s\n", buf);
// 	int k = 0;
// 	while (k < j)
// 	{
// 		printf("start pos %d\n", start[k]);
// 		k++;
// 	}
// 	k = 0;
// 	while (k < dollar)
// 	{
// 		printf("dollar pos %d\n", dollar_pos[k]);
// 		char *tmp = ft_substr(value, dollar_pos[k] + 1, )
// 		k++;
// 	}
// 	return (buf);
// }