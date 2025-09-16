#include "minishell.h"
#include "libft.h"

t_handler_qt    *new_handler_node(int start)
{
    t_handler_qt    *node;

    node = malloc(sizeof(t_handler_qt));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_handler_qt));
	node->start_qt_input = start;
    node->end_qt_input = 0;
    node->start_qt_buf = 0;
    node->end_qt_buf = 0;
    node->dollar = 0;
    node->part = NULL;
    node->next = NULL;
    return (node);
}

void    add_handler_lst_back(t_handler_qt **handler, t_handler_qt *node)
{
    t_handler_qt    *cur;

    if (!*handler)
        *handler = node;
    else
    {
        cur = *handler;
        while (cur->next)
            cur = cur->next;
        cur->next = node;
    }
}

void    free_handler_lst(t_handler_qt **handler)
{
    t_handler_qt *cur;
    t_handler_qt *next;

    cur = *handler;
    while (cur)
    {
        next = cur->next;
        if (cur->part)
            free(cur->part);
        free(cur);
        cur = next;
    }
    handler = NULL;
}