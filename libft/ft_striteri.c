/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:54:38 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/07 12:23:10 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdio.h>
/*
void ft_put_upper_ch(unsigned int n, char *s)
{
    (void)n;

    if (*s >= 'a' && *s <= 'z') {
        *s -= 32;
    }
    write(1, s, 1);
}

size_t	ft_strlen(char *str)
{
	char	*p;

	p = str;
	while (*p)
		p++;
	return (p - str);
}*/

void	ft_striteri(char *s, void (*f)(unsigned int, char *s))
{
	unsigned int	n;

	if (!s)
		return ;
	n = 0;
	while (s[n])
	{
		f(n, &s[n]);
		n++;
	}
}
/*
int	main()
{
	char str[] = "hello world";
	ft_striteri(str, ft_put_upper_ch);
}*/			
