#include "minishell.h"
#include "libft.h"

void	child_process(t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data, t_parser *parser)
{
	int	infd;
	int	outfd;
	t_redir	*tmp;

	if (cur->command->invalid_in)
		exit(1);
	if (cur->command->infile)
	{
		infd = open(cur->command->infile, O_RDONLY);
		if (infd < 0)
			exit(1);
		dup2(infd, STDIN_FILENO);
		close(infd);
	}
	//else if (cur == *head_cmd && pipe_data->f_fds[0] != -1)
	// {
	// 	dup2(pipe_data->f_fds[0], 0);
	// 	close(pipe_data->f_fds[0]);
	// }
	else if (pipe_data->prev_pipe != -1)
	{
		dup2(pipe_data->prev_pipe, 0);
		close(pipe_data->prev_pipe);
	}
	if (cur->command->outfile)
	{
		tmp = cur->command->outfile;
		while (tmp->next)
		{
			if (tmp->invalid)
				exit(1);
			tmp = tmp->next;
		}
		if (tmp->invalid)
			exit(1);
		if (tmp->append)
			outfd = open(tmp->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			outfd = open(tmp->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfd < 0)
			exit(1);
		dup2(outfd, STDOUT_FILENO);
		close(outfd);
		if (cur->next == NULL && pipe_data->pipefd[1] != -1)
		{
			// dup2(pipe_data->f_fds[1], 1);
			close(pipe_data->pipefd[1]);
		}
	}
	// else if (cur->next == NULL && pipe_data->f_fds[1] != -1)
	// {
	// 	dup2(pipe_data->f_fds[1], 1);
	// 	close(pipe_data->f_fds[1]);
	// }
	else if (cur->next != NULL)
	{
		dup2(pipe_data->pipefd[1], 1);
		close(pipe_data->pipefd[1]);
	}
	if (pipe_data->pipefd[0] != -1)
		close(pipe_data->pipefd[0]);
	execute_cmd(&(*head_cmd)->var_list, cur->command, pipe_data->envp, parser);
}

void	parent_process(t_cmdlist *cur, t_pipex *pipe_data)
{
	if (pipe_data->prev_pipe != -1)
		close(pipe_data->prev_pipe);
	if (cur->next != NULL)
	{
		close(pipe_data->pipefd[1]);
		pipe_data->prev_pipe = pipe_data->pipefd[0];
	}
	else
	{
		if (pipe_data->pipefd[0] != -1)
			close(pipe_data->pipefd[0]);
	}
}

pid_t	fork_and_pid(t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data, t_parser *parser)
{
	pid_t		pid;

	if (cur->next != NULL)
	{
		if (pipe(pipe_data->pipefd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	pid = fork();
	if (pid < 0)
	{
		perror("pipe");
		return (-1);
	}
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
	g_exit_status = wait_child_process(last_pid);
}
