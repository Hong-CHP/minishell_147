/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <hporta-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:01:44 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/08 18:28:21 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

char	*ft_strchr(const char *s, int c)
{
	const char			*p;
	unsigned char		ch;

	p = s;
	ch = (unsigned char)c;
	while (*p)
	{
		if (*p == ch)
			return ((char *)p);
		p++;
	}
	if (ch == '\0')
		return ((char *)p);
	return (NULL);
}
/*
int	main()
{
	const char *str = "a b c";
	int c = '\0';
	char *p = strchr(str, c);
	char *p1 = ft_strchr(str, c);
	
	printf("%s\n", p);
	printf("%s", p1);
}*/
