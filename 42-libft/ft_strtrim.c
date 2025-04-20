/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:05:40 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:50:08 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_str;
	size_t	start;
	size_t	end;

	if (!s1)
		return (NULL);
	if (!set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (s1[start] && ft_strchr(set, s1[start]))
	{
		start++;
	}
	while (s1[end] && ft_strchr(set, s1[end]))
	{
		end--;
	}
	new_str = ft_substr(s1, start, end - start + 1);
	return (new_str);
}
// Remove caracteres no inicio e no final
/*
int main()
{
    char *s1 = "xxHelloxx";
    char *set = "x";

    printf("%s\n", ft_strtrim(s1, set));
}
*/
