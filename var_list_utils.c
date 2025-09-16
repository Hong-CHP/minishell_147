#include "minishell.h"
#include "libft.h"

int	if_export_variable(char *content)
{
	if (ft_strncmp("export", content, 6) == 0)
		return (1);
	return (0);
}

int ft_list_size(t_varlist **head)
{
    int count;
	t_varlist	*cur;
    
    count = 0;
    cur = *head;
    while (cur)
    {
        count++;
        cur = cur->next;
    }
    return (count);
}

void	clean_var_list(t_varlist **head)
{
    t_varlist   *cur;
    t_varlist   *next;
    
    cur = *head;
    while (cur)
    {
        next = cur->next;
        if (cur->var_data)
        {
            free(cur->var_data->var);
            free(cur->var_data->val);
            free(cur->var_data);
        }
        free(cur);
        cur = next;
    }
    *head = NULL;
}

void	add_var_lst_order(t_varlist **head, t_varlist *var_node)
{
    t_varlist   *cur;
    t_varlist   *prev;

    if (!var_node)
        return ;
    if (!*head || ft_strcmp(var_node->var_data->var, (*head)->var_data->var) < 0)
    {
        var_node->next = *head;
        *head = var_node;
        return ;
    }
    prev = *head;
    cur = (*head)->next;
    while (cur && ft_strcmp(var_node->var_data->var, cur->var_data->var) > 0)
    {
        prev = cur;
        cur = cur->next;
    }
    prev->next = var_node;
    var_node->next = cur;
}

void	add_var_lst_back(t_varlist **head, t_varlist *var_node)
{
    t_varlist   *cur;

    if (!*head)
        *head = var_node;
    else
    {
        cur = *head;
        while (cur->next)
            cur = cur->next;
        cur->next = var_node;
        var_node = var_node->next;
        var_node = NULL;
    }
}