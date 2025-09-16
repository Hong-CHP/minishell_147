#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

extern int g_exit_status;
typedef struct s_cmdlist t_cmdlist;

typedef enum e_tk_type 
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEMICOLON,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EOF,
	TOKEN_ERROR,
}				t_tk_type;

typedef struct s_token 
{
    t_tk_type		type;
    char			*value;
    struct s_token	*next;
}				t_token;

typedef struct s_parser
{
	t_token *tokens;
    t_token *current;
	char	*input;
	size_t	pos;
	bool	error;
	char	*error_msg;
}				t_parser;

typedef struct s_variable
{
	char	*var;
	char	*val;
	int		exported;
	struct s_variable *next;
}				t_variable;

typedef struct	s_varlist
{
	t_variable			*var_data;
	struct s_varlist	*next;
}				t_varlist;

typedef struct	s_redir
{
	char			*name;
	int				append;
	int				invalid;
	struct s_redir	*next;
}				t_redir;

typedef struct	s_command
{
	char	*cmd;
	char	**args;
	int		argc;
	char	*infile;
	int		invalid_in;
	t_redir	*outfile;
	int		here_doc;
	char	*limiter;
}				t_command;

typedef struct	s_cmdlist
{
	t_command			*command;
	t_varlist			*var_list;
	struct	s_cmdlist	*next;
}				t_cmdlist;

typedef struct s_pipex 
{
	int		prev_pipe;
	int		pipefd[2];
	int		f_fds[2];
	char	**envp;
}				t_pipex;

typedef struct s_handler_qd
{
	int					start_qt_input;
	int					end_qt_input;
	int					start_qt_buf;
	int					end_qt_buf;
	int					dollar;
	char				*part;
	struct s_handler_qd	*next;
}				t_handler_qt;

typedef struct s_no_rep_dol
{
	int		*start;
	int		*end;
	int		idx;
}				t_no_rep_dol;

//readline_utils.c
int		has_unclosed_quote(char *input);
//minishell.c
void	minishell(char *input, t_varlist **head_var, char **envp);
//parsing.c
t_cmdlist	*parse_cmd_line(t_parser *parser, t_varlist **head_var);
t_cmdlist	*parse_pipeline(t_parser *parser, t_varlist **head_var);
//parsing_utils.c
int		ft_strcmp(char *s1, char *s2);
void	skip_whitespace(t_parser *parse);
void	add_cmdlist_back(t_cmdlist **head, t_cmdlist *cmd_node);
void	clean_cmdlist(t_cmdlist **head);
//var_list.c
void	create_var_list(t_varlist **head, char *input, t_command *cmd_node);
int		create_var_list_or_find_node(t_varlist **head, char *input, t_cmdlist **head_cmd);
int		is_valide_varname(char *input);
int		process_var_val_export(t_varlist **head, char *input, t_variable *var_node, t_command *cmd_node);
//var_list_utils.c
int		if_export_variable(char *content);
void	add_var_lst_order(t_varlist **head, t_varlist *var_node);
int		ft_list_size(t_varlist **head);
void	clean_var_list(t_varlist **head);
void	add_var_lst_back(t_varlist **head, t_varlist *var_node);
//var_control.c
char	*if_variable_var(char *input);
char	*if_variable_val(char *input);
//tokenize_utils.c
int		if_quote(char *str);
void	free_token_list(t_token **head);
char	*extract_value_if_sign(char *value, t_varlist **head_var);
//var_val.c
void	fill_variable_value(char *content, char *var, char *val);
int 	init_registre_variable(t_variable *var_dt, char *input, t_varlist **head_var);
char	*extract_value(char *val, t_varlist **head_var);
//extract_cmd_utils.c
int		if_slash_trans(char *str);
char	*find_words_in_single_quote(char *content, char ch);
char	*find_words_in_quote(char *content, char ch);
char	*find_words_in_slash(char *content);
char	*copy_str_without_slash(char *dest, char *src, int len);
char	*dup_str_without_slash(char *src, int len);
//tokenize.c
t_token	*tokenize(t_parser *parser, t_varlist **head_var);
t_token *create_token(t_tk_type type, char *value);
//tokenize_type.c
t_token	*create_pipe_token(t_parser *parser);
t_token	*create_input_token(t_parser *parser);
t_token	*create_output_token(t_parser *parser);
t_token	*create_and_token(t_parser *parser);
t_token	*create_simple_token(t_parser *parser, int type, const char *s);
//extract_word.c
char	*extract_word(t_parser *parser, t_varlist **head_var);
char	*extract_by_type_sign(char *buf, t_parser *parser, t_handler_qt **handler);
//extract_word_utils.c
int				if_still_space(char *res);
t_handler_qt    *new_handler_node(int start);
void    		add_handler_lst_back(t_handler_qt **handler, t_handler_qt *node);
void    free_handler_lst(t_handler_qt **handler);
//dollar_sign.c
char	*reg_dollar_sign(char *str, t_varlist **head_var);
//dollar_sign_utils.c
int		if_dollar_sign(char *str);
int		is_varname_format(char *str);
void	free_vars_vals(char **vars, char **vals);
//process_token.c
int		is_cmd_token(int type);
int		process_token(t_parser *p, t_cmdlist *cmd);
int		handle_word(t_parser *p, char **args, int *argc);
// process_token_utils.c
int		is_cmd_token(int type);
int		set_error(t_parser *p, char *msg, int err_code);
void	ft_put_err_msg(t_parser *parser, char *str, char *file_name);
//pipex.c
void	execute_pipeline(t_cmdlist **head_cmd, t_pipex *pipe_data, t_parser *parser);
//pipex_utils.c
int		if_pipex(t_cmdlist **head_cmd);
t_pipex	*init_pipe_data(t_pipex *pipe_data, char **envp);
int		get_in_out_files_fd(t_cmdlist **head, t_pipex *pipe_data, t_parser *parser);
int		invalide_syntaxe_token(t_parser *parser, t_token *tokens);
//checker_files_access.c
int		check_infile_permission(t_parser *parser, char *infile);
int		check_outfile_permission(t_parser *parser, char *outfile);
//build_in.c
int		if_buildin(char *cmd);
int		execute_builtin(t_varlist **head_var, t_command *cmd_node, char **ev, int sub_process);
int		builtin_export(t_varlist **head_var, t_command *cmd_node, int sub_process);
//execute_cmd.c
void	execute_cmd(t_varlist **head_var, t_command *command, char **ev, t_parser *parser);
void	execute_single_cmd(t_varlist **head_var, t_command *cmd, t_pipex *pipe_data, t_parser *parser);
//execute_cmd_utils.c
void	update_exit_status(int status);
int		if_slash(char *str);
char	**find_sign_then_split(char *str);
void	free_split(char **strs);
//execute_here_doc.c
int		execute_here_doc(t_cmdlist **head_cmd, t_pipex *pipe_data);
//get_next_line.c
char	*get_next_line(int fd);
//count_size_of_envp.c
void	create_var_list_for_ev(t_varlist **head, char *input);
// void	set_varlist_exported(t_varlist **head_var);
void	create_var_list_for_ev(t_varlist **head, char *input);
int		count_size_of_envp(char **envp);
void	print_all_variable_in_list(t_varlist **head);
//build_in_cmd.c
int		builtin_echo(t_command *cmd);
int		builtin_cd(t_command *cmd);
int		builtin_pwd(void);
int		builtin_env(char **ev, t_varlist **head);
int		builtin_exit(t_command *cmd);

#endif