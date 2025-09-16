/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:48:17 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/07 10:41:18 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int	ft_itoa_len(int n)
{
	long long	nb;
	int			len;

	len = 0;
	nb = (long long)n;
	if (n <= 0)
	{
		nb = -nb;
		len++;
		if (n == 0)
			return (len);
	}
	while (nb > 0)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long long	nb;
	int			len;
	char		*p;

	len = ft_itoa_len(n);
	p = (char *)malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	nb = (long long)n;
	if (n < 0)
	{
		nb = -nb;
		p[0] = '-';
	}
	p[len] = '\0';
	if (n == 0)
		p[0] = '0';
	while (len > 0 && nb > 0)
	{
		len--;
		p[len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (p);
}
/*
int	main()
{
	//ft_itoa_len(-2314);
	printf("%s", ft_itoa(-10));
}*/
