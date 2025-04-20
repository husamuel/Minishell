/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:48:34 by husamuel          #+#    #+#             */
/*   Updated: 2024/10/27 17:15:18 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}
// Adiciona um novo nó ao inicio de uma lista
/*
int main()
{
  t_list *list = NULL;

  t_list *node1 = ft_lstnew("First");
  t_list *node2 = ft_lstnew("Second");
  t_list *node3 = ft_lstnew("Third");

  ft_lstadd_front(&list, node1);
  ft_lstadd_front(&list, node2);
  ft_lstadd_front(&list, node3);

  while (list)
  {
    printf("%s\n", (char *)list->content);
    list = list -> next;
  } 
}
*/
