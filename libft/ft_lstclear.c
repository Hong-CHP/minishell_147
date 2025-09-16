/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <hporta-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:21:35 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/08 12:07:10 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*curr;
	t_list	*next_node;

	curr = *lst;
	while (curr)
	{
		next_node = curr->next;
		del(curr->content);
		free(curr);
		curr = next_node;
	}
	*lst = NULL;
}
