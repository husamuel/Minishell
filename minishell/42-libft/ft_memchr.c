/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:28:39 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:25:06 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*string;

	string = (const unsigned char *)s;
	while (n)
	{
		if (*string == (unsigned char)c)
			return ((void *)string);
		string++;
		n--;
	}
	return (NULL);
}
// Localiza a primeira ocorrência de um caractere em um bloco de memória
/*
int main() {

    const char *str = "Hello World";
    int c = 'W';
    size_t n = ft_strlen(str);

    char *result = ft_memchr(str, c, n);
    printf("%c\n", *result);
}
*/
