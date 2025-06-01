/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_settings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:38:31 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/26 14:14:23 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	g_exit_status = sig + 128;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_redisplay();
}

void	sigint_cmd_handler(int sig)
{
	g_exit_status = sig + 128;
	write(1, "\n", 1);
}

void	sigquit_handler(int sig)
{
	g_exit_status = sig + 128;
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}

void	setup_exec_signals(void)
{
	signal(SIGINT, sigint_cmd_handler);
	signal(SIGQUIT, SIG_IGN);
}
