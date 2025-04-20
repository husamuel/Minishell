/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:22:22 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:04:58 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
/*
int main()
{
	printf("%d\n", ft_isalnum(90));
	printf("%d\n", ft_isalnum(245));
	printf("%d\n", ft_isalnum(3));
}
*/
// Verifica se o caractere é alfanumérico
