#include "minishell.h"
#include "libft.h"

int		check_dir_validate(char *part_path)
{
	DIR		*dir;

	dir = opendir(part_path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

char	**extract_path(t_varlist **head_var)
{
	t_varlist	*cur;
	char		**ev_path;

	cur = *head_var;
	while (cur)
	{
		if (ft_strcmp("PATH", cur->var_data->var) == 0)
		{
			ev_path = find_sign_then_split(cur->var_data->val);
			if (!ev_path)
			{
				perror("Extract path no success");
				free_split(ev_path);
				return (NULL);
			}
			return (ev_path);
		}
		cur = cur->next;
	}
	return (NULL);
}

char	*get_path_after_join(char **args, char **ev_path, int i)
{
	char	*temp;
	char	*path;

	temp = NULL;
	path = NULL;
	temp = ft_strjoin("/", args[0]);
	if (!temp)
	{
		free_split(ev_path);
		return (NULL);
	}
	path = ft_strjoin(ev_path[i], temp);
	free(temp);
	return (path);
}

char	*search_in_path(char **args, char **ev_path)
{
	int		i;
	char	*path;
	char	*exe_path;

	exe_path = NULL;
	i = 0;
	while (ev_path[i])
	{
		path = get_path_after_join(args, ev_path, i);
		if (path)
		{
			if (access(path, X_OK) == 0)
			{
				exe_path = ft_strdup(path);
				free(path);
				break ;
			}
			free(path);
		}
		i++;
	}
	return (exe_path);
}

char	*find_exe_path(char **args, t_varlist **head_var)
{
	char	*exe_path;
	char	**ev_path;

	exe_path = NULL;
	ev_path = extract_path(head_var);
	if (!ev_path)
		return (NULL);
	exe_path = search_in_path(args, ev_path);
	free_split(ev_path);
	return (exe_path);
}
