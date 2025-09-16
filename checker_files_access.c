#include "minishell.h"
#include "libft.h"

int		check_infile_permission(t_parser *parser, char *infile)
{
	if (access(infile, F_OK) == -1)
		return (set_error(parser, "No such file or directory", 1));
	if (access(infile, R_OK) == -1)
		return (set_error(parser, "Permission denied", 1));
	return (0);
}

int		check_outfile_permission(t_parser *parser, char *outfile)
{
	char	*slash;
	char	*path;

	if (access(outfile, F_OK) == 0)
	{
		if (access(outfile, W_OK) == -1)
			return (set_error(parser, "Permission denied", 1));
	}
	else
	{
		slash = ft_strrchr(outfile, '/');
		if (slash)
		{
			path = ft_substr(outfile, 0, ft_strlen(outfile) - ft_strlen(slash));
			if (access(path, F_OK) == -1)
				return (set_error(parser, "No such file or directory", 1));
			else if (access(path, F_OK) == 0)
			{
				if (access(path, W_OK) == -1)
					return (set_error(parser, "Permission denied", 1));
			}
			free(path);
		}
	}
	return (0);
}
