/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:18:08 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 18:49:22 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;

	if (*needle == 0)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return (NULL);
	while (*haystack && len >= needle_len)
	{
		if (ft_strncmp(haystack, needle, needle_len) == 0)
			return ((char *)haystack);
		haystack++;
		len--;
	}
	return (NULL);
}
// Procura a primeira occorrencia de um substring
// dentro de um string e retorna a posicao
/* int main(void) {
    const char *haystack = "Hello, world!";
    const char *needle = "orld";
    size_t len = ft_strlen(haystack);
    char *result1 = ft_strnstr(haystack, needle, len);
    printf("%s\n", result1);
} */
