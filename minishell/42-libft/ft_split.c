/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:23:53 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:31:43 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_allocate(char **tab, char const *s, char sep)
{
	char const	*tmp;
	char		**tab_tmp;

	tab_tmp = tab;
	tmp = s;
	while (*tmp)
	{
		while (*s == sep)
			s++;
		tmp = s;
		while (*tmp && *tmp != sep)
			tmp++;
		if (*tmp == sep || tmp > s)
		{
			*tab_tmp = ft_substr(s, 0, tmp - s);
			s = tmp;
			tab_tmp++;
		}
	}
	*tab_tmp = NULL;
}

static int	ft_count_words(char const *s, char sep)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == sep)
			s++;
		if (*s)
			count++;
		while (*s && *s != sep)
			s++;
	}
	return (count);
}

char	**ft_split(char const *s, char sep)
{
	char	**str;
	int		size;

	if (!s)
		return (NULL);
	size = ft_count_words(s, sep);
	str = malloc(sizeof(char *) * (size + 1));
	if (!str)
		return (NULL);
	ft_allocate(str, s, sep);
	return (str);
}
// Divide uma sring em várias strings
/* int	main()
{
	char	str[] = "um as fgr flg rf";
	char	c = ' ';
	char	**spt = ft_split(str, c);
	int	i = 0;
	while (*spt)
	{
		ft_putstr_fd(*spt, 1);
		spt++;
	}
	ft_putchar_fd('\n', 1);
} */
