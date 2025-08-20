/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:41:35 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 18:03:02 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ptr1;
	const unsigned char	*ptr2;

	ptr1 = (const unsigned char *)s1;
	ptr2 = (const unsigned char *)s2;
	while (n)
	{
		if (*ptr1 != *ptr2)
		{
			return (*ptr1 - *ptr2);
		}
		ptr1++;
		ptr2++;
		n--;
	}
	return (0);
}
// Compara dois blocos de memória
/*
int main()
{
    char *s1 = "111";
    char *s2 = "222";
    char *s3 = "333";
    printf("%d\n%d\n%d\n", ft_memcmp(s2, s1, 1), 
    ft_memcmp(s1, s3, 1)), ft_memcmp(s3, s2, 1);
}
*/
