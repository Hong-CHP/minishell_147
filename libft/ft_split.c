/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hporta-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 09:56:39 by hporta-c          #+#    #+#             */
/*   Updated: 2025/04/07 12:49:54 by hporta-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int	is_char(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

int	count_words(char *str, char c)
{
	int	is_ch;
	int	count;

	count = 0;
	is_ch = 1;
	while (*str)
	{
		if (!(*str == c))
		{
			if (is_ch == 1)
			{
				count++;
				is_ch = 0;
			}
		}
		else
			is_ch = 1;
		str++;
	}
	return (count);
}

char	*allocate_copy_tab(int word_len, char *str)
{
	char	*p;
	char	*word;

	p = (char *)malloc((word_len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	word = p;
	p[word_len] = '\0';
	while (word_len > 0)
	{
		*p++ = *str++;
		word_len--;
	}
	return (word);
}

char	**ft_fill_split(char **tab, char *str, char c, int *tab_i)
{
	int	word_len;
	int	i;

	word_len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			if (word_len > 0)
			{
				tab[*tab_i] = allocate_copy_tab(word_len, &str[i - word_len]);
				(*tab_i)++;
				word_len = 0;
			}
		}
		else
			word_len++;
		i++;
	}
	if (word_len > 0)
		tab[(*tab_i)++] = allocate_copy_tab(word_len, &str[i - word_len]);
	return (tab);
}

char	**ft_split(char	const *s, char c)
{
	char	**tab;
	char	*str;
	int		tab_i;

	tab_i = 0;
	str = (char *)s;
	tab = (char **)malloc((count_words(str, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	ft_fill_split(tab, str, c, &tab_i);
	tab[tab_i] = NULL;
	return (tab);
}
/*
int	main()
{
	char str[] = ", hello, my, new, world,";
	char ch = ',';
	int	i;
	int j;
	char **s = ft_split(str, ch);
	i = 0;
	if (s)
	{
		while (s[i])
		{
			j = 0;
			while (s[i][j])
			{
				printf("%c", s[i][j]);
				j++;
			}
			i++;
		}
		i = 0;
		while (s[i])
		{	
			free(s[i]);
			i++;
		}
		free(s);
	}
}*/
