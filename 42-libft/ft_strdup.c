/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:24:36 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/29 21:30:47 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	size;
	char	*dest;

	size = ft_strlen(src) + 1;
	dest = malloc(size);
	if (!dest)
		return (NULL);
	ft_memcpy(dest, src, size);
	return (dest);
}
// Duplica uma string
/*
int main()
{
    char *str = "Hello World";
    char *dest = strdup(str);
    printf("%s\n", dest);
}
*/
