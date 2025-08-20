/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:26:13 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:10:44 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_get_size(int n)
{
	int	digits;

	digits = 0;
	if (n <= 0)
		digits++;
	while (n != 0)
	{
		n = n / 10;
		digits++;
	}
	return (digits);
}

static void	ft_fill(int digits, int offset, char *str, int n)
{
	while (digits > offset)
	{
		str[digits - 1] = (n % 10) + '0';
		n /= 10;
		digits--;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		digits;
	int		offset;

	offset = 0;
	digits = ft_get_size(n);
	str = (char *)malloc(sizeof(char) * digits + 1);
	if (!str)
		return (0);
	if (n == -2147483648)
	{
		str[0] = '-';
		str[1] = '2';
		n = 147483648;
		offset = 2;
	}
	if (n < 0)
	{
		str[0] = '-';
		offset = 1;
		n = -n;
	}
	ft_fill(digits, offset, str, n);
	str[digits] = '\0';
	return (str);
}
/*
int main()
{
    printf("%s\n", ft_itoa(42));
    printf("%s\n", ft_itoa(-2134556));
    printf("%s\n", ft_itoa(0));
}
*/
// Converte um número inteiro em uma string
