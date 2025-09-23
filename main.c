#include "minishell.h"
#include "libft.h"
#include <signal.h>

int catch = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	if (catch == 1)
	{
		return ;
	}
	ft_putchar_fd('\n', 1);
	rl_replace_line("", 0); 
	rl_on_new_line();
	rl_redisplay();
}

void	setup_sigaction(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	read_the_line(char *input, t_varlist **head_var, char **envp, int *g_exit)
{
	while (1)
	{
		catch = 0;
		input = readline("\1\033[1;33m\2minishell\1\033[0m\2> ");
		if (input == NULL)
		{
			rl_on_new_line();
			printf("exit\n");
			break ;
		}
		else if (*input == '\0')
		{
			free(input);
			continue;
		}
		else if (ft_strcmp(input, "history -c") == 0)
			rl_clear_history(); 
		else if (*input)
		{
			add_history(input);
			minishell(input, head_var, envp, g_exit);
			free(input);
		}
	}
}

// minishell fill_word_with_real_val extract_word handle_word process_var_val_export
int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_varlist	*head_var;
	int			envp_size;
	int			i;
	int			g_exit;

	(void)argv;
	g_exit = 0;
	if (argc != 1)
		return (1);
	envp_size = count_size_of_envp(envp);
	head_var = NULL;
	input = NULL;
	setup_sigaction();
	i = 0;
	while (i < envp_size)
	{
		create_var_list_for_ev(&head_var, envp[i]);
		i++;
	}
	read_the_line(input, &head_var, envp, &g_exit);
	clean_var_list(&head_var);
	return (0);
}
