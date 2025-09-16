/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:46:21 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/01 14:08:56 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p;
	unsigned char		ch;

	ch = (unsigned char)c;
	p = (const unsigned char *)s;
	while (n > 0)
	{
		if (*p == ch)
			return ((void *)p);
		p++;
		n--;
	}
	return (NULL);
}
/*
int	main()
{
	const char *str = "GeeksforGeeks";
	int c = 's';
//	char *p = strchr(str, c);
//	char *p1 = ft_strchr(str, c);
//	printf("%s\n", p);
	printf("%s", ft_memchr(str, 's', 10));
}*/
