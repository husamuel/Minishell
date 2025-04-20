/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:08:42 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:14:30 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp_node;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp_node = ft_lstlast(*lst);
		tmp_node->next = new;
	}
}
// Adiciona um novo nó ao final de uma lista
/*
int main()
{
    t_list *list = NULL;

    t_list *node1 = ft_lstnew("First");
    t_list *node2 = ft_lstnew("Second");
    t_list *node3 = ft_lstnew("Third");

    ft_lstadd_back(&list, node1);
    ft_lstadd_back(&list, node2);
    ft_lstadd_back(&list, node3);

    t_list *current = list;
    while (current)
    {
        printf("%s\n", (char *)current->content);
        current = current->next;
    }
}
*/
