#include "minishell.h"
#include "libft.h"

int ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

void skip_whitespace(t_parser *parse)
{
	while (parse->input[parse->pos] && 
		(parse->input[parse->pos] == ' ' || 
		 parse->input[parse->pos] == '\t' || 
		 parse->input[parse->pos] == '\n'))
		parse->pos++;
}

void	clean_cmdlist(t_cmdlist **head)
{
	t_cmdlist	*cur;
	t_cmdlist	*next;

	cur = *head;
	while (cur)
	{
		next = cur->next;
		free(cur->command);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

void	add_cmdlist_back(t_cmdlist **head, t_cmdlist *cmd_node)
{
	t_cmdlist	*cur;

	cur = NULL;
	if (!*head)
		*head = cmd_node;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = cmd_node;
	}
}