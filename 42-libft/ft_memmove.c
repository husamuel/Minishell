/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:28:47 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 19:06:06 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	const unsigned char	*s;
	unsigned char		*d;

	if (dst == NULL && src == NULL)
	{
		return (dst);
	}
	if (dst < src)
		return (ft_memcpy(dst, src, n));
	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	while (n)
	{
		n--;
		d[n] = s[n];
	}
	return (dst);
}
// Copia um bloco de memória de uma localização para outra
// permitindo que a origem e o destino se sobreponham.

/*
int main()
{
	char *src = "Hello World";
	char *dest;
	size_t n = ft_strlen(src);
	ft_memcpy(dest, src, n);
	printf("%s\n", dest);
}
*/
