/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:04:20 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:18:55 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst -> next)
	{
		lst = lst->next;
	}
	return (lst);
}
// Devolve o último nó de uma lista
/* int main() {

    t_list *list = NULL;
    t_list *node1 = ft_lstnew("First");
    t_list *node2 = ft_lstnew("Second");
    t_list *node3 = ft_lstnew("Third");

    ft_lstadd_back(&list, node1);
    ft_lstadd_back(&list, node2);
    ft_lstadd_back(&list, node3);

    t_list *last_node = ft_lstlast(list);
    printf("%s\n", last_node->content);
} */
