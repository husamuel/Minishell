/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:11:59 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/29 21:30:44 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*ptr;

	ptr = malloc(size * num);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, num * size);
	return (ptr);
}
// Aloca memória para um array inicializando com 0
/*
int main()
{
    int *array = (int *)calloc(10, sizeof(int));
    for(int i = 0; i < 10; i++)
    {
        printf("%d", array[i]);
    }
}
*/
