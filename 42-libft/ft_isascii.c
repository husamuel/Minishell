/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:24:30 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/23 13:06:26 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
/*
int main()
{
	printf("%d\n", ft_isascii(90));
	printf("%d\n", ft_isascii(245));
	printf("%d\n", ft_isascii(3));
}
*/
// Verificar se o caractere é um dos caracteres ASCII
