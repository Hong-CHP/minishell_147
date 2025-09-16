/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:53:40 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/07 12:48:18 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*p;
	char const	*p1;
	char const	*p2;
	char		*res;

	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	p1 = (char const *)s1;
	p2 = (char const *)s2;
	p = (char *)malloc((ft_strlen(p1) + ft_strlen(p2) + 1) * sizeof(char));
	if (!p)
		return (NULL);
	res = p;
	while (*p1)
		*res++ = *p1++;
	while (*p2)
		*res++ = *p2++;
	*res = '\0';
	return (p);
}
/*
int	main()
{
	char *s1 = NULL;
	char *s2 = "un join simple";
	printf("%s\n", ft_strjoin(s1, s2));
}*/
