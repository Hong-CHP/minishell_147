/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 09:29:02 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/01 10:28:56 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*res;

	res = dest;
	if (dest < src)
	{
		while (n > 0)
		{
			*(char *)dest++ = *(char *)src++;
			n--;
		}
	}
	else
	{
		while (n > 0)
		{
			*(char *)(dest + n - 1) = *(char *)(src + n - 1);
			n--;
		}
	}
	return (res);
}
/*
int main()
{

    char str[] = "hello world!";
    printf("%s\n", ft_memmove(str, str + 1, 4));
    char str1[] = "hello world!";
    printf("%s\n", ft_memmove(str1 + 1, str1, 4));

    char str2[] = "hello world!";
    printf("%s\n", memmove(str2, str2 + 1, 4));
    char str3[] = "hello world!";
    printf("%s\n", memmove(str3 + 1, str3, 4)); 
    return (0);
}*/
