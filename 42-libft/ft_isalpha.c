/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:13:50 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/23 13:03:56 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_mai(int c)
{
	return ('A' <= c && c <= 'Z');
}

static int	ft_min(int c)
{
	return ('a' <= c && c <= 'z');
}

int	ft_isalpha(int c)
{
	return (ft_mai(c) || ft_min(c));
}
/*
int main()
{
	printf("%d\n", ft_isalpha(90));
	printf("%d\n", ft_isalpha(245));
	printf("%d\n", ft_isalpha(3));
}
*/
// Verifica se é uma letra do alfabeto
