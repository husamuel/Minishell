/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:30:19 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:21:39 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;

	head = NULL;
	while (lst)
	{
		tmp = ft_lstnew((*f)(lst->content));
		if (!tmp)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, tmp);
		lst = lst -> next;
	}
	return (head);
}
// Aplica uma função a cada elemento de uma lista
// encadeada e cria uma nova lista com os resultados

/* void *print_string(void *content) {
    printf("%s\n", (char *)content + 1); 
    return NULL; 
}

void free_string(void *content) {
    free(content);
}

int main() {
    t_list *list = NULL;
    t_list *node1 = ft_lstnew(strdup("Hello"));
    t_list *node2 = ft_lstnew(strdup("World"));

    ft_lstadd_back(&list, node1);
    ft_lstadd_back(&list, node2);
    
    printf("Lista original:\n");
    t_list *temp = list;
    while (temp) {
        printf("%s\n", (char *)temp->content);
        temp = temp->next;
    }
    printf("Nova lista:\n");

    ft_lstmap(list, print_string, free_string);
    ft_lstclear(&list, free_string);
} */
