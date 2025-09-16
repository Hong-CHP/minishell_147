#include "minishell.h"
#include "libft.h"

int		builtin_cd(t_command *cmd)
{
	char	*path;
	char	*home;

	if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (cmd->argc == 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		path = home;
	}
	else
		path = cmd->args[1];
	if (chdir(path) == -1)
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
			{
				ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
				return (2);
			}
			i++;
		}
		exit_code = ft_atoi(cmd->args[1]);
	}
	ft_putstr_fd("exit\n", 1);
	exit(exit_code);
}

int		check_echo_n_in_args(char *str)
{
	int	i;

	if (ft_strncmp(str, "-n", 2) == 0)
		i = 2;
	else
		i = 0;
	while (str[i])
	{
		if (str[i] != 'n')
		{
			printf("catch: %c\n", str[i]);
			return (0);
		}
		i++;
	}
	return (1);
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
		{
			while (cmd->args[i])
			{
				ft_putstr_fd(cmd->args[i], 1);
				if (i < cmd->argc - 1)
					ft_putstr_fd(" ", 1);
				i++;
			}
		}
		else if (ft_strncmp(cmd->args[i], "-n", 2) == 0 && !check_echo_n_in_args(cmd->args[i]))
		{
			ft_putstr_fd(cmd->args[i], 1);
			if (i < cmd->argc - 1)
				ft_putstr_fd(" ", 1);
		}
		else
			newline = 0;
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
