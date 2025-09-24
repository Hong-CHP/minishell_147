#include "minishell.h"
#include "libft.h"

int		if_pipex(t_cmdlist **head_cmd)
{
	t_cmdlist	*cur;
	int			pipe; 
	
	pipe = 0;
	cur = *head_cmd;
	while(cur)
	{
		pipe++;
		cur = cur->next;
	}
	return (pipe);
}

t_pipex	*init_pipe_data(char **envp)
{
	t_pipex *pipe_data;

	pipe_data = malloc(sizeof(t_pipex));
	if (!pipe_data)
		return (NULL);
	pipe_data->f_fds[0] = -1;
	pipe_data->f_fds[1] = -1;
	pipe_data->envp = envp;
	pipe_data->prev_pipe = -1;
	return (pipe_data);
}

pid_t	fork_and_pid(t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data, t_parser *parser)
{
	pid_t		pid;

	if (cur->next != NULL)
	{
		if (pipe(pipe_data->pipefd) == -1)
			return (pipe_fork_error());
	}
	pid = fork();
	if (pid < 0)
		return (pipe_fork_error());
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		child_process(head_cmd, cur, pipe_data, parser);
		exit (0);
	}
	else
	{
		parent_process(cur, pipe_data);
		return (pid);
	}
}

int	wait_child_process(pid_t last_pid)
{
	int	exit_code;
	int	new_status;
	pid_t	pid;

	exit_code = 0;
	pid = wait(&new_status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(new_status))
				exit_code = WEXITSTATUS(new_status);
			else if (WIFSIGNALED(new_status))
			{
				exit_code = 128 + WTERMSIG(new_status);
				if (exit_code == 130)
					ft_putchar_fd('\n', 1);
			}
		}
		pid = wait(&new_status);
	}
	return (exit_code);
}

void	execute_pipeline(t_cmdlist **head_cmd, t_pipex *pipe_data, t_parser *parser)
{
	t_cmdlist	*cur;
	pid_t		last_pid;

	cur = *head_cmd;
	// printf("pipe_data fd = %d, %d\n", pipe_data->f_fds[0], pipe_data->f_fds[1]);
	while (cur)
	{
		last_pid = fork_and_pid(head_cmd, cur, pipe_data, parser);
		cur = cur->next;
	}
	*parser->g_exit_status = wait_child_process(last_pid);
}
