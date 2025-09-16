/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:48:14 by hporta-c          #+#    #+#             */
/*   Updated: 2025/03/31 14:59:00 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

void	ft_bzero(void *s, size_t n)
{
	char	*p;
	size_t	i;

	i = 0;
	p = (char *)s;
	while (i < n)
	{
		p[i] = '\0';
		i++;
	}
	return ;
}
/*
int	main()
{
	char str[] = "hello world";
	int	arr[] = {23, 20, 25}; 
	ft_bzero(arr, sizeof(arr));
	bzero(str, 5);
	printf("%s", str);
}*/
