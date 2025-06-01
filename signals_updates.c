/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_updates.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:39:48 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/26 14:29:04 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_exec_signals(void);

void	disable_ctrl_backslash(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	restore_child_tty(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag |= ECHOCTL;
	term.c_cc[VQUIT] = 28;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	is_core_dumped(int status)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 18);
		rl_on_new_line();
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
	}
	g_exit_status = 128 + sig;
	return (g_exit_status);
}
