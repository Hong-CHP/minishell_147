#include "minishell.h"
#include "libft.h"

int		get_cd_home_path(char **path, t_varlist **head)
{
	char	*home;
	t_varlist	*cur;

	home = NULL;
	cur = *head;
	while (cur)
	{
		if (ft_strcmp(cur->var_data->var, "HOME") == 0)
			home = cur->var_data->val;
		cur = cur->next;
	}
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	*path = home;
	return (0);
}

int		check_cd_path_err(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	if (access(path, F_OK) == 0 && access(path, R_OK) == -1)
	{
		ft_putstr_fd(": Permission denied\n", 2);
		return (1);
	}
	ft_putstr_fd(": No such file or directory\n", 2);
	return (1);
}

void	unset_variable_from_list(t_varlist **head_var, t_varlist **cur, t_varlist **prev)
{
	t_varlist	*tmp;

	if ((*prev))
		(*prev)->next = (*cur)->next;
	else
		*head_var = (*cur)->next;
	tmp = (*cur)->next;
	free((*cur)->var_data->val);
	free((*cur)->var_data->var);
	free((*cur)->var_data);
	free((*cur));
	(*cur) = tmp;
}

int		put_exit_err_msg(void)
{
	ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
	return (2);
}