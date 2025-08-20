/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:53:40 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/26 13:54:24 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_fork_error(int stdout_backup)
{
	perror("minishell: fork");
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	setup_signals();
	return (1);
}
