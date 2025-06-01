/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:22:31 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/12 16:18:29 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_envp(char **envp);
void	create_node(t_env **head, char *s);

t_mini	init(char **envp)
{
	t_mini	ms;

	ms.input = NULL;
	ms.export = get_envp(envp);
	ms.token = NULL;
	ms.state = NULL;
	ms.prompt = GREEN "minishell" RESET "$ ";
	ms.exit_status_count = 0;
	ms.count = 0;
	ms.in_quotes = 0;
	ms.pipe = 0;
	ms.none = 0;
	ms.redirect = 0;
	ms.expr = 0;
	ms.output = NULL;
	ms.current_cmd = NULL;
	ms.heredoc = 0;
	ms.echo = 0;
	return (ms);
}

t_env	*get_envp(char **envp)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		create_node(&head, envp[i]);
		i++;
	}
	return (head);
}

char	*get_var(char *s)
{
	int		i;
	char	*var;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '=')
		i++;
	var = malloc(i + 1);
	i = 0;
	while (s[i] != '=')
	{
		var[i] = s[i];
		i++;
	}
	var[i] = '\0';
	return (var);
}

t_env	*ft_last(t_env *head)
{
	t_env	*current;

	if (!head)
		return (NULL);
	current = head;
	while (current && current->next)
		current = current->next;
	return (current);
}
