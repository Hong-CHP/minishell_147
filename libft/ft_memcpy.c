/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 09:30:34 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/01 09:55:23 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	void	*res;

	res = dest;
	while (n > 0)
	{
		*(char *)dest++ = *(char *)src++;
		n--;
	}
	return (res);
}
/*
int	main()
{
	char str[] = "hello world!";
	printf("%s\n", ft_memcpy(str, str + 1, 4));
	char str1[] = "hello world!";
	printf("%s\n", ft_memcpy1(str1 + 1, str1, 4));
	char str2[] = "hello world!";
	printf("%s\n", memcpy(str2, str2 + 1, 4));
	char str3[] = "hello world!";
	printf("%s\n", memcpy(str3 + 1, str3, 4));
	return (0);
}*/
