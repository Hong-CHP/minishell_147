/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:51:39 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/01 12:45:59 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>
/*
int	ft_strncmp1(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	char	*p1;
	char	*p2;
	
	i = 0;
	p1 = (char *)s1;
	p2 = (char *)s2;
	while (i < n && *p1 && *p2)
	{
		if (*p1 != *p2)
			return ((unsigned char)*p1 - (unsigned char)*p2);
		p1++;
		p2++;
		i++;
	}
	if (i < n)
			return ((unsigned char)*p1 - (unsigned char)*p2);
	return (0);
}
*/
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}
/*
int	main()
{
	char *s1 = "pomme";
	char *s2 = "poire";
	printf("%d\n", ft_strncmp(s1, s2, 4));
	char *s3 = "abc";
	char *s4 = "abcd";
	printf("%d\n", ft_strncmp(s3, s4, 4));
    char *s5 = "pomme";
    char *s6 = "poire";
    printf("%d\n", ft_strncmp(s5, s6, 4));
    char *s7 = "abc";
    char *s8 = "abcd";
    printf("%d\n", ft_strncmp(s7, s8, 4));
	char *s9 = "pomme";
    char *s10 = "poire";
    printf("%d\n", strncmp(s1, s2, 4));
    char *s11 = "abc";
    char *s12 = "abcd";
    printf("%d\n", strncmp(s3, s4, 4));
	return (0);
}*/
