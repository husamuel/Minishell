/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:08:37 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/23 19:08:45 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	exec_env(t_token *token, t_mini *mini)
{
	t_env	*ev;

	(void)token;
	ev = mini->export;
	while (ev)
	{
		if (ev->var && ev->content) // garante que ambas existem
			printf("%s=%s\n", ev->var, ev->content);
		ev = ev->next;
	}
}
