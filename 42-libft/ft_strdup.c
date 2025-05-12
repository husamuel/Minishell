/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:02:26 by gtretiak          #+#    #+#             */
/*   Updated: 2024/11/08 14:40:29 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	l;
	char	*s;
	char	*temp;

	l = 0;
	temp = (char *)s1;
	while (s1[l])
		l++;
	s = malloc(sizeof(char) * (l + 1));
	if (s == NULL)
		return (NULL);
	temp = s;
	while (*s1)
		*temp++ = *s1++;
	*temp = '\0';
	return (s);
}
