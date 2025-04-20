/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:07:17 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:03:36 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}
//Zera os primeiros n bytes da memória apontada por s.
/*
int main(void)
{
	char buffer[10] = "ABCDEFGHIJ";
	printf("Antes de ft_bzero: ");

    for (int i = 0; i < 10; i++)
	{
        printf("%c ", buffer[i]);
    }
    printf("\n");

    ft_bzero(buffer, 5);

	printf("Depois de ft_bzero: ");

    for (int i = 0; i < 10; i++) {
		if (buffer[i] == '\0')
			printf("* ");
		else
        	printf("%c", buffer[i]);
    }

    printf("\n");
}
*/
