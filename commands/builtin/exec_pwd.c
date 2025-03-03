/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruida-si <ruida-si@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:34:42 by ruida-si          #+#    #+#             */
/*   Updated: 2025/02/13 18:36:13 by ruida-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	exec_pwd(t_token *token)
{
	char	*current_path;
	char	buffer[BUFFER_SIZE];

	(void)token;
	current_path = getcwd(buffer, BUFFER_SIZE);
	if (!current_path)
	{
		perror("pwd");
		return ;
	}
	printf("%s\n", current_path);
}
