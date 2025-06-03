/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:00:57 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/03 15:35:26 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	handle_question_mark(int *i, t_mini *mini)
{
	printf("%s", mini->token->args[1]);
	*i += 2;
}

static void	handle_digit(int *i, char *input)
{
	if (input[*i + 1] == '0')
		printf("minishell");
	*i += 2;
}

static void	handle_alpha(int *i, char *input, t_mini *mini)
{
	char	*var;

	var = ft_strdup(&input[++(*i)]);
	print_echo(input, i, var, mini);
}

void	echo_dollar(int *i, char *input, t_mini *mini)
{
	if (input[*i + 1] == '?')
		handle_question_mark(i, mini);
	else if (ft_isdigit(input[*i + 1]))
		handle_digit(i, input);
	else if (ft_isalpha(input[*i + 1]))
		handle_alpha(i, input, mini);
	else if (input[*i] == '$' && mini->redirect > 0)
		mini->token->args_file[1] = ft_strdup("$");
	else
	{
		printf("%c", input[*i]);
		(*i)++;
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
