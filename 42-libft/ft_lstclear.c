/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:56:15 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:16:37 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;

	if (lst == NULL || del == NULL)
	{
		return ;
	}
	head = *lst;
	while (head)
	{
		tmp = head -> next;
		del(head -> content);
		free(head);
		head = tmp;
	}
	*lst = NULL;
}

// Liberta a memória de uma lista aplicando a funcao em cada nó

/* void del_content(void *content)
{
    free(content);
}

int main()
{
    t_list *list = NULL;

    t_list *node1 = ft_lstnew(strdup("First"));
    t_list *node2 = ft_lstnew(strdup("Second"));
    t_list *node3 = ft_lstnew(strdup("Third"));

    ft_lstadd_front(&list, node1);
    ft_lstadd_front(&list, node2);
    ft_lstadd_front(&list, node3);

    t_list *current = list;
    while (current)
    {
        printf("%s\n", (char *)current->content);
        current = current->next;
    }
    ft_lstclear(&list, del_content);
    return 0;
} */
