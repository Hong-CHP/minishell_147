/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <hporta-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:43:08 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/07 19:05:01 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int	is_set(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*dst;

	if (!s1)
		return (NULL);
	if (s1[0] == '\0')
		return (ft_strdup(""));
	if (!set || set[0] == '\0')
		return (ft_strdup(s1));
	start = 0;
	while (is_set(s1[start], set) == 1)
		start++;
	end = ft_strlen(s1) - 1;
	while (is_set(s1[end], set) == 1)
		end--;
	if (end < start)
		return (ft_strdup(""));
	dst = ft_substr(s1, start, end - start + 1);
	return (dst);
}
