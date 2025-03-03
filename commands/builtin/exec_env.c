/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruida-si <ruida-si@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:38:34 by ruida-si          #+#    #+#             */
/*   Updated: 2025/03/01 14:48:29 by ruida-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	exec_env(t_token *token, t_mini *mini)
{
	t_env	*ev;
	int		i;

	i = 0;
	ev = mini->export;
	(void)token;
	while (ev)
	{
		if (ev->content)
		{
			printf("%s\n", ev->content);
			i++;
		}
		ev = ev->next;
	}
	printf("%i\n", i);
}
// Destroy -u --unset Create VAR1=valor1 PATH=/custom/path ls