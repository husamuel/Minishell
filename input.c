/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:24:30 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/26 14:14:17 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(t_mini *ms, char *prompt)
{
	char	*input;

	(void)ms;
	input = readline(prompt);
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		exit(0);
	}
	if (!handle_input_line(input, ms))
	{
		ms->echo = 1;
	}
	if (input && *input)
		add_history(input);
	return (input);
}

void	ft_update_ms(t_mini *ms)
{
	ms->pipe = 0;
	ms->exit_status_count = 0;
	ms->count++;
	ms->none = 0;
}
