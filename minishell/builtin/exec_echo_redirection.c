/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:41:42 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 12:43:10 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*print_echo(char *input, int *i, char *var, t_mini *mini);
void	echo_others(t_token *next, int i, t_mini *mini, char *input);
void	echo_dollar(int *i, char *input, t_mini *mini);
int		check_nl_echo(char *s);

int	has_redirection(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (!current->is_literal && !current->quoted
			&& (current->type == CMD_REDIRECT_OUT
				|| current->type == CMD_REDIRECT_IN
				|| current->type == CMD_HEREDOC
				|| current->type == CMD_APPEND))
			return (1);
		current = current->next;
	}
	return (0);
}

void	ft_handle_redirect_case(char *input_start, char *redirect_pos)
{
	int		len_to_redirect;
	char	*temp_input;

	len_to_redirect = redirect_pos - input_start;
	temp_input = ft_custom_substr(input_start, 0, len_to_redirect);
	printf("\n");
	free(temp_input);
}

char	*ft_get_redirect_pos(char *input_start)
{
	char	*redirect_pos;

	redirect_pos = NULL;
	redirect_pos = ft_custom_strstr(input_start, " >");
	if (!redirect_pos)
		redirect_pos = ft_custom_strstr(input_start, " >>");
	if (!redirect_pos)
		redirect_pos = ft_custom_strstr(input_start, " <");
	if (!redirect_pos)
		redirect_pos = ft_custom_strstr(input_start, " <<");
	return (redirect_pos);
}
