/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:09:06 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 12:43:24 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

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
