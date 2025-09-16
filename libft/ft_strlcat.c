/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <hporta-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:52 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/08 17:28:05 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	char	*p;
	size_t	i;

	d_len = ft_strlen(dest);
	s_len = ft_strlen(src);
	p = dest;
	if (size <= d_len)
		return (s_len + size);
	p = dest + d_len;
	i = 0;
	while ((size - d_len - 1) > i && src[i])
	{
		p[i] = src[i];
		i++;
	}
	p[i] = '\0';
	return (d_len + s_len);
}
