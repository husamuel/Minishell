/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:28:35 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:37:44 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
// Aplica uma função f a cada caractere de uma string s
/*
void example_function(unsigned int index, char *c) {
    printf("Index: %u, Char: %c\n", index, *c);
}
int main()
{
    char *str = "Hello World";
    ft_striteri(str, example_function);
}
*/
