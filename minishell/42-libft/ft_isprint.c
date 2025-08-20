/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:26:15 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/23 13:08:57 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	else
		return (0);
}
/*
int main()
{
	printf("%d\n", ft_isprint(50));
	printf("%d\n", ft_isprint(245));
	printf("%d\n", ft_isprint(3));
}
*/
// Verifica se o caractere é printável