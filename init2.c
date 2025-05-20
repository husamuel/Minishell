/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:43:09 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 19:43:12 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*init_env_node(char *s)
{
	t_env	*node;
	char	*equal_sign;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal_sign = ft_strchr(s, '=');
	if (equal_sign)
	{
		node->var = ft_substr(s, 0, equal_sign - s);
		node->content = ft_strdup(equal_sign + 1);
	}
	else
	{
		node->var = ft_strdup(s);
		node->content = NULL;
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

static void	append_env_node(t_env **head, t_env *node)
{
	t_env	*last;

	if (!(*head))
		*head = node;
	else
	{
		last = ft_last(*head);
		node->prev = last;
		last->next = node;
	}
}

void	create_node(t_env **head, char *s)
{
	t_env	*node;

	node = init_env_node(s);
	if (!node)
		return ;
	append_env_node(head, node);
}
