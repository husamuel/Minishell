/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:17:06 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/21 12:17:10 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_mini	ms;
	int		result;

	(void)(argc);
	(void)(argv);
	result = 0;
	ms = init(envp);
	disable_ctrl_backslash();
	setup_signals();
	while (1)
	{
		ms.input = get_input(&ms, ms.prompt);
		ms.token = lexer(ms.input);
		if (ms.token)
		{
			result = parser(&ms);
			if (result == 0)
				ft_handle_zero(&ms);
			exec(&ms);
			free_tokens(ms.token);
		}
		ft_update_ms(&ms);
	}
	free(&ms);
	exit(EXIT_SUCCESS);
}
