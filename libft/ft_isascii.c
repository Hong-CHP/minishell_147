/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 09:47:12 by hporta-c          #+#    #+#             */
/*   Updated: 2025/03/31 09:47:47 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
/*
int	main()
{
	printf("%d ", ft_isascii(0));
	printf("%d ", ft_isascii(128));
	printf("%d ", ft_isascii(67));
	printf("%d", ft_isascii(127));
}*/
