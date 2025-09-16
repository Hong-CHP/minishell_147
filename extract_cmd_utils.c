#include "minishell.h"
#include "libft.h"

int		if_slash_trans(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			count++;
		i++;
	}
	return (count);
}

char	*copy_str_without_slash(char *dest, char *src, int len)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while (i < len)
	{
		if (src[i] == '\\')
			i++;
		else
		{
			dest[j] = src[i];
			j++;
			i++;
		}
	}
	dest[j] = '\0';
	return (dest);
}

char	*dup_str_without_slash(char *src, int len)
{
	char	*dest;

	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	copy_str_without_slash(dest, src, len);
	return (dest);
}

char	*find_words_in_slash(char *content)
{
	int		i;
	int		start;
    size_t  len;
	char	*str;
	//malloc need to be free then
	
    len = ft_strlen(content);
	i = 0;
	str = NULL;
	start = 0;
	while (i < (int)len && content[i] != '\\')
		i++;
	if (content[i] == '\0')
		return (NULL);
	i++;
	start = i;
	while (i < (int)len && content[i] != '\\')
		i++;
	str = malloc(sizeof(char) * (i - start + 1));
	if (!str)
		return (NULL);
	copy_str_without_slash(str, &(content[start]), i - start + 2);
	return (str);
}

// char	*find_words_in_single_quote(char *content, char ch)
// {
// 	int		i;
// 	int		start;
//     size_t  len;
// 	char	*str;
// 	//malloc need to be free then
	
//     len = ft_strlen(content);
// 	i = 0;
// 	str = NULL;
// 	start = 0;
// 	while (i < (int)len && content[i] != ch)
// 		i++;
	// if (content[i] == '\0')
	// 	return (NULL);
	// i++;
	// start = i;
// 	while (i < (int)len && content[i] != ch)
// 		i++;
// 	if(content[i] == '\0')
// 		return (NULL);
// 	str = malloc(sizeof(char) * (i - start + 1));
// 	if (!str)
// 		return (NULL);
// 	ft_strlcpy(str, &(content[start]), i - start + 1);
// 	return (str);
// }

char	*find_words_in_single_quote(char *src, char ch)
{
	char	*dest;
	size_t		i;
	int		j;

	dest = malloc(sizeof(char) * ft_strlen(src));
	if (!dest)
		return (NULL);
	j = 0;
	i = 0;
	while (i < ft_strlen(src))
	{
		if (src[i] == ch)
			i++;
		else
		{
			dest[j] = src[i];
			j++;
			i++;
		}
	}
	dest[j] = '\0';
	return (dest);
}

char	*find_words_in_quote(char *src, char ch)
{
	char	*dest;
	size_t		i;
	int		j;

	dest = malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!dest)
		return (NULL);
	j = 0;
	i = 0;
	while (i < ft_strlen(src))
	{
		if (src[i] == ch)
			i++;
		else if (src[i] == '\\' && src[i + 1] == ch)
		{
			i++;
			dest[j] = src[i];
			j++;
		}
		else
		{ 
			dest[j] = src[i];
			j++;
			i++;
		}
	}
	dest[j] = '\0';
	return (dest);
}

// char	*find_words_in_quote(char *content, char ch)
// {
// 	int		i;
// 	int		start;
//     size_t  len;
// 	char	*str;
// 	//malloc need to be free then
// 	char	*slash_ptr;
	
//     len = ft_strlen(content);
// 	i = 0;
// 	str = NULL;
// 	start = 0;
// 	while (i < (int)len && content[i] != ch)
// 		i++;
// 	if (content[i] == '\0')
// 		return (NULL);
// 	i++;
// 	start = i;
// 	while (i < (int)len && content[i] != ch)
// 	{
// 		if (content[i] == '\\' && content[i + 1] == ch)
// 			i++;
// 		i++;
// 	}
// 	if(content[i] == '\0')
// 		return (NULL);
// 	str = malloc(sizeof(char) * (i - start + 1));
// 	if (!str)
// 		return (NULL);
// 	slash_ptr = ft_strchr(content, '\\');
// 	if (slash_ptr)
// 	{
// 		printf("slash_ptr is %s\n", slash_ptr);
// 		if (*(slash_ptr + 1) && *(slash_ptr + 1) == ch)
// 			copy_str_without_slash(str, &(content[start]), i - start);
// 		else
// 			ft_strlcpy(str, &(content[start]), i - start + 1);
// 	}
// 	else
// 		ft_strlcpy(str, &(content[start]), i - start + 1);
// 	return (str);
// }
