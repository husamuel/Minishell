/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:17:06 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/20 20:47:30 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_parser(t_parser *state)
{
	state->curr = NULL;
	state->prev = NULL;
	state->last_cmd = NULL;
	state->cmd_seen = 0;
}

static void	main_loop(t_mini *ms, t_parser *state)
{
	int	result;

	while (1)
	{
		ms->input = get_input(ms, ms->prompt);
		if (!ms->input)
			break ;
		ms->token = lexer(ms->input);
		if (ms->token)
		{
			result = parser(state, ms);
			if (result == 0)
				ft_handle_zero(ms);
			exec(ms);
			free_tokens(ms->token);
			ms->token = NULL;
		}
		ft_update_ms(ms);
	}
}

int	main(int argc, char *argv[], char **envp)
{
	t_parser	state;
	t_mini		ms;

	(void)(argc);
	(void)(argv);
	ms = init(envp);
	ft_init_parser(&state);
	ms.state = &state;
	disable_ctrl_backslash();
	setup_signals();
	main_loop(&ms, &state);
	if (ms.export)
		free_env_list(ms.export);
	ft_free_minishell(&ms, 0);
	exit(EXIT_SUCCESS);
}
