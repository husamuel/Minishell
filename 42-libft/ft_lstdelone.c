/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:42:28 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:17:28 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL || del == NULL)
		return ;
	del(lst->content);
	free(lst);
}
// Remove um único nó de uma lista

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

    ft_lstadd_back(&list, node1);
    ft_lstadd_back(&list, node2);
    ft_lstadd_back(&list, node3);

    printf("Lista original:\n");
    t_list *temp = list;
    while (temp)
    {
        printf("%s\n", temp->content);
        temp = temp -> next;
    }
    printf("\nRemovendo o segundo nó:\n");
    t_list *temp2 = list;
    while (temp2->next != node2)
    {
        temp2 = temp2->next;
    }
    temp2->next = node2->next;
    ft_lstdelone(node2, del_content);
    temp = list;
    while (temp)
    {
        printf("%s\n", (char *)temp->content);
        temp = temp->next;
    }

} */
