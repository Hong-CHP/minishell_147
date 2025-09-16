/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:52:39 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/07 11:05:28 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdio.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	if (n == 0)
		return (0);
	while (n > 0)
	{
		if (*p1 != *p2)
			return ((unsigned char)*p1 - (unsigned char)*p2);
		p1++;
		p2++;
		n--;
	}
	return (0);
}
/*
int	main()
{
	char *s1 = "pomme";
	char *s2 = "poire";
	printf("%d\n", ft_memcmp(s1, s2, 4));
	char *s3 = "abcde";
	char *s4 = "abcd";
	printf("%d\n", ft_memcmp(s3, s4, 4));
	char *s9 = "pomme";
    char *s10 = "poire";
    printf("%d\n", memcmp(s1, s2, 4));
    char *s11 = "abcde";
    char *s12 = "abcd";
    printf("%d\n", memcmp(s3, s4, 4));
}*/
