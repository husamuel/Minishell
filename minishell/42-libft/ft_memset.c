/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:32:49 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:26:55 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	size_t			i;

	i = 0;
	ptr = (unsigned char *)b;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (b);
}
// Preenche um bloco de memória com um valor específico.
/*
int main()
{
	unsigned char ptr[10];
	ft_memset(ptr, 4, 10);
	for(int i = 0; i < 10; i++)
	{
		printf("%d", ptr[i]);
	}
}
*/
