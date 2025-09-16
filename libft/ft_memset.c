/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:19:32 by hporta-c          #+#    #+#             */
/*   Updated: 2025/03/31 11:48:35 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*p;
	char	c1;

	i = 0;
	p = (char *)s;
	c1 = (char)c;
	while (i < n)
	{
		p[i] = c1;
		i++;
	}
	return ((void *)p);
}
/*
int	main()
{
	char str[] = "geeksforgeeks is for programming geeks.";
	printf("%s\n", ft_memset(str, '.', 8));
	printf("%s", memset(str, '.', 8));
}*/
