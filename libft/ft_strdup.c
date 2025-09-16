/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <hporta-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:13:41 by hporta-c          #+#    #+#             */
/*   Updated: 2025/08/20 10:41:21 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	const char	*p;
	char		*dst;
	char		*ptr;

	if (!s)
		return (NULL);
	p = (const char *)s;
	dst = (char *)malloc((ft_strlen(p) + 1) * sizeof(char));
	if (!dst)
		return (NULL);
	ptr = dst;
	while (*p)
		*ptr++ = *p++;
	*ptr = '\0';
	return (dst);
}
/*
int	main()
{
	char *str = "hello world!";
	printf("%s\n", ft_strdup(str));	
}*/	
