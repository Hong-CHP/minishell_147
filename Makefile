NAME = minishell
LIBFT_DIR = libft
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(LIBFT_DIR)

LIBFT = $(LIBFT_DIR)/libft.a
LIB = -lreadline

SRCS = main.c minishell.c var_list.c var_val.c var_control.c \
		parsing.c tokenize.c tokenize_types.c extract_word.c build_in.c \
		dollar_sign.c process_token.c pipex.c checker_files_access.c \
		execute_cmd.c execute_here_doc.c get_next_line.c export_vars.c \
		build_in_cmd.c \
		readline_utils.c extract_cmd_utils.c dollar_sign_utils.c pipex_utils.c \
		execute_cmd_utils.c extract_word_utils.c \
		tokenize_utils.c var_list_utils.c parsing_utils.c process_token_utils.c 
OBJS = $(SRCS:.c=.o)

RM = rm -f

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIB) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: fclean clean all re