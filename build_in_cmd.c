#include "minishell.h"
#include "libft.h"

int		builtin_cd(t_command *cmd, t_varlist **head)
{
	char	*path;

	if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (get_cd_path(cmd, &path, head) != 0)
		return (1);
	if (path && cmd->args[1] && !cmd->args[1][0] && cmd->cmd[3] != '$')
	{
		free(path);
		return (0);
	}
	if (chdir(path) == -1)
		return (check_cd_path_err(path));
	return (0);
}

int		builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("minishell: pwd: error getting current directory\n", 2);
		return (1);
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	free(cwd);
	return (0);
}

//Iterate through env array and print each BUT
//Might need to handle arguments
int	builtin_env(char **ev, t_varlist **head)
{
	t_varlist	*cur;
	(void)ev;

	cur = *head;
	while (cur)
	{
		if (cur->var_data->exported == 1)
			printf("%s=%s\n", cur->var_data->var, cur->var_data->val);
		cur = cur->next;
	}
	return (0);
}

// I think this one is fine
int		builtin_exit(t_command *cmd)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (cmd->argc > 1)
	{
		i = 0;
		while (cmd->args[1][i])
		{
			if (cmd->args[1][i] == '+' || cmd->args[1][i] == '-')
				i++;
			if (!(cmd->args[1][i] >= '0' && cmd->args[1][i] <= '9'))
				return (put_exit_err_msg());
			i++;
		}
		exit_code = ft_atoi(cmd->args[1]);
	}
	ft_putstr_fd("exit\n", 1);
	exit(exit_code);
}

int		builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (i < cmd->argc)
	{
		if (ft_strncmp(cmd->args[i], "-n", 2) != 0)
			put_echo_content(cmd, &i);
		else if (ft_strncmp(cmd->args[i], "-n", 2) == 0
			&& !check_echo_n_in_args(cmd->args[i]))
			put_echo_content(cmd, &i);
		else
			newline = 0;
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
