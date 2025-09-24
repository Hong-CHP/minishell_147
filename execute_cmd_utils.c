#include "minishell.h"
#include "libft.h"

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
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
}

int	no_executable_if_empty(t_command *cmd, t_parser *parser)
{
	if (!cmd)
		return (0);
	if (cmd->cmd && !*cmd->cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (cmd->invalid_in)
	{
		ft_put_err_msg(parser, NULL, cmd->infile);
		return (0);
	}
	return (1);
}

// void	update_exit_status(int status, t_parser *parser)
// {
// 	if (WIFEXITED(status))
// 		*parser->g_exit_status = WEXITSTATUS(status);
// 	else if (WIFSIGNALED(status))
// 	{
// 		*parser->g_exit_status = 128 + WTERMSIG(status);
// 		if (*parser->g_exit_status == 130)
// 			ft_putchar_fd('\n', 1);
// 	}
// 	else
// 		*parser->g_exit_status = 1;
// }