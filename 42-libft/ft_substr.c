/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:36:42 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/29 21:33:58 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	str_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
	{
		substr = malloc(1);
		if (substr)
			substr[0] = '\0';
		return (substr);
	}
	if (len > str_len - start)
		len = str_len - start;
	substr = malloc(len + 1);
	if (!substr)
		return (NULL);
	ft_strlcpy (substr, s + start, len + 1);
	return (substr);
}
// Extrai uma substring de uma string a partir do comeco e do tamanho
/* int main()
{
    char *s = "Hello World!";
    int start = 3;
    size_t len = 5;
    printf("%s\n", ft_substr(s, start, len));
} */
