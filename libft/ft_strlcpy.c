/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:29:59 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/07 15:24:37 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	if (src == NULL)
		return (0);
	while (src[len])
		len++;
	if (!dst || size == 0)
		return (len);
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}
/*
int main()
{
	char dest[20];
	char *str = "hello world!";
	
	printf("%zu\n", ft_strlcpy(dest, str, 20));
	printf("%s", dest);
}
int main(void)
{
	char	dst[10] = {1, 1, 0, 1, 1, 1, 1, 1, 1, 0};
	char	src[10] = {5, 5, 5, 0, 0, 0, 0, 0, 0, 0};

	printf("strlcpy returned %ld\n", ft_strlcpy(dst, src, 0));
	for (int i = 0; i < 10; i++)
		printf("%d ", dst[i]);
}*/
