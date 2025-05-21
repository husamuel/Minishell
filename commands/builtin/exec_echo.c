/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:49 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/21 09:43:18 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

char	*print_echo(char *input, int *i, char *var, t_mini *mini);
void	echo_others(t_token *next, int i, t_mini *mini, char *input);
void	echo_dollar(int *i, char *input, t_mini *mini);
int		check_nl_echo(char *s);

static void	ft_handle_home(void)
{
	char	*s;

	s = getenv("HOME");
	printf("%s\n", s);
}

static void	handle_echo_output(t_token *next, t_mini *mini, int has_redirect)
{
	char	*input_start;
	char	*redirect_pos;

	input_start = mini->input + 5;
	redirect_pos = NULL;
	if (has_redirect)
		redirect_pos = ft_get_redirect_pos(input_start);
	if (has_redirect && redirect_pos)
		ft_handle_redirect_case(input_start, redirect_pos);
	else
		echo_others(next, 0, mini, input_start);
}

void	exec_echo(t_token *token, t_mini *mini)
{
	t_token	*next;
	int		has_redirect;

	has_redirect = has_redirection(token);
	next = token->next;
	if (!next)
		printf("\n");
	else if (mini->echo != 0)
		return ;
	else if (ft_strcmp(next->cmd, "~") == 0)
		ft_handle_home();
	else if (ft_strcmp(next->cmd, "$?+$?") == 0)
		return ;
	else
		handle_echo_output(next, mini, has_redirect);
}

void	echo_dollar(int *i, char *input, t_mini *mini)
{
	char	*var;

	var = NULL;
	if (input[*i + 1] == '?')
	{
		printf("%s", mini->token->args[1]);
		*i += 2;
		return ;
	}
	else if (ft_isdigit(input[*i + 1]))
	{
		if (input[*i + 1] == '0')
			printf("minishell");
		*i += 2;
	}
	else if (ft_isalpha(input[*i + 1]))
	{
		var = ft_strdup(&input[++(*i)]);
		print_echo(input, i, var, mini);
	}
}

char	*print_echo(char *input, int *i, char *var, t_mini *mini)
{
	char	*s;
	int		j;

	j = 0;
	while (ft_isalnum(var[j]) || var[j] == '_')
		j++;
	var[j] = '\0';
	s = expand_var(var, mini->export);
	free(var);
	if (s)
	{
		printf("%s", s);
		free(s);
	}
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	return (s);
}
