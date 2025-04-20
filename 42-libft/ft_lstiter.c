/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:20:02 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 18:11:32 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
// Aplica uma funcao a cada nó da lista

/* void print_content(void *content) {
	printf("%s\n", (char *)content);
}

int main() {

	t_list *list = NULL;
	t_list *node1 = ft_lstnew("First");
	t_list *node2 = ft_lstnew("Second");
	t_list *node3 = ft_lstnew("Third");

	ft_lstadd_back(&list, node1);
	ft_lstadd_back(&list, node2);
	ft_lstadd_back(&list, node3);

	ft_lstiter(list, print_content);
}
*/
