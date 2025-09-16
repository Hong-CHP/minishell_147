/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <hporta-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:33:37 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/08 18:30:04 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
/*
char	to_upper(unsigned int n, char c)
{
	(void)n;

	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
} 

size_t	ft_strlen(char *str)
{
	char	*p;

	p = str;
	while (*p)
		p++;
	return (p - str);
}*/

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*p;
	char			*res;
	unsigned int	n;

	if (!s)
		return (NULL);
	res = (char *)malloc((ft_strlen((char *)s) + 1) * sizeof(char));
	if (!res)
		return (NULL);
	p = res;
	n = 0;
	while (*s)
		*res++ = f(n++, *s++);
	*res = '\0';
	return (p);
}
/*
int	main()
{
	char str[] = "hello world";
	char *new_str = ft_strmapi(str, to_upper);

	printf("%s", new_str);
}*/
