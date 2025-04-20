/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:20:35 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/23 13:07:47 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
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
	printf("%d\n", ft_isdigit(50));
	printf("%d\n", ft_isdigit(245));
	printf("%d\n", ft_isdigit(3));
}
*/
// Verifica se o caractere é um digito