#include "minishell.h"
#include "libft.h"

void	update_exit_status(int status)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (g_exit_status == 130)
			ft_putchar_fd('\n', 1);
	}
	else
		g_exit_status = 1;
}

int	if_slash(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			count++;
		i++;
	}
	return (count);
}

char	**sep_split(char *str, char sep)
{
	char	**res;

	if (sep != 0)
		res = ft_split(str, sep);
	else
	{
		res = malloc(sizeof(char *) * 2);
		if (!res)
			return (NULL);
		res[0] = ft_strdup(str);
		res[1] = NULL;
	}
	return (res);
}

char	**find_sign_then_split(char *str)
{
	int		i;
	char	**res;
	char	sep;

	sep = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		{
			sep = str[i];
			break ;
		}
		else if (str[i] == ':')
		{
			sep = str[i];
			break ;
		}
		i++;
	}
	res = sep_split(str, sep);
	return (res);
}

void	free_split(char **strs)
{
	int i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
}