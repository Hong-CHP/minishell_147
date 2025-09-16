#include "minishell.h"
#include "libft.h"

char	*cut_line(char *dest, char **rest)
{
	char	*line;
	int		end;

	if (!dest)
		return (NULL);
	end = 0;
	while (dest[end] != '\n' && dest[end] != '\0')
		end++;
	if (dest[end] == '\n')
		line = ft_substr(dest, 0, end + 1);
	else
		line = ft_substr(dest, 0, end);
	if (dest[end] != '\0' && dest[end + 1] != '\0')
		*rest = ft_strdup(&dest[end + 1]);
	else
		*rest = NULL;
	return (line);
}

char	*allo_init_buf(int fd, char **dest, int buf_size)
{
	char	*buff;
	int		bytes_read;
	char	*temp;

	buff = malloc(buf_size + 1);
	if (!buff)
		return (NULL);
	if (!*dest)
		*dest = ft_strdup("");
	bytes_read = 1;
	while (bytes_read > 0 && ft_strchr(*dest, '\n') == NULL)
	{
		bytes_read = read(fd, buff, buf_size);
		if (bytes_read <= 0)
		{
			free(buff);
			return (*dest);
		}
		buff[bytes_read] = '\0';
		temp = *dest;
		*dest = ft_strjoin(*dest, buff);
		free(temp);
	}
	free(buff);
	return (*dest);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*rest;
	static char	*dest;
    int     buf_size;

    buf_size = 5;
	if (!allo_init_buf(fd, &dest, buf_size))
		return (NULL);
	if (!dest || dest[0] == '\0')
	{
		free(dest);
		dest = NULL;
		return (NULL);
	}
	line = cut_line(dest, &rest);
	free(dest);
	dest = rest;
	return (line);
}