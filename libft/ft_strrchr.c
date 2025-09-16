/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <hporta-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:29:20 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/08 18:31:44 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	const char		*p;
	int				len;
	char			*res;
	unsigned char	ch;

	if (s == NULL)
		return (NULL);
	p = s;
	res = (char *)p;
	ch = (unsigned char)c;
	len = ft_strlen(res);
	if (ch == '\0')
		return (res + len);
	while (len > 0)
	{
		len--;
		if (res[len] == ch)
			return (res + len);
	}
	return (NULL);
}
/*
int	main()
{
	char str[] = "bonjour";
	
	printf("%s\n", ft_strrchr(str, 's'));
	printf("%s\n", strrchr(str, 's'));
}*/
