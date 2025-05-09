/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:54:49 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/26 18:45:17 by gtretiak         ###   ########.fr       */
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

void	exec_echo(t_token *token, t_mini *mini)
{
	t_token	*next;
	int		has_redirect;
	char	*input_start;
	char	*redirect_pos;

	has_redirect = has_redirection(token);
	next = token->next;
	if (!next)
		printf("\n");
	else if (ft_strcmp(next->cmd, "~") == 0)
		ft_handle_home();
	else
	{
		input_start = mini->input + 5;
		redirect_pos = NULL;
		if (has_redirect)
			redirect_pos = ft_get_redirect_pos(input_start);
		if (has_redirect && redirect_pos)
			ft_handle_redirect_case(input_start, redirect_pos);
		else
			echo_others(next, 0, mini, input_start);
	}
}

void	echo_dollar(int *i, char *input, t_mini *mini)
{
	char	*var;

	var = NULL;
	if (input[*i + 1] == '?')
	{
		printf("0");
		*i += 2;
		if (input[*i] == ' ')
			(*i)++;
		return ;
	}
	else if (ft_isdigit(input[*i + 1]))
	{
		if (input[*i + 1] == '0')
			printf("minishell");
		*i += 2;
		if (input[*i] == ' ' && input[*i - 1] != '0')
			(*i)++;
	}
	else if (ft_isalpha(input[*i + 1]))
	{
		var = ft_strdup(&input[++(*i)]);
		if (!print_echo(input, i, var, mini) && input[*i] == ' ')
			(*i)++;
	}
}

char	*print_echo(char *input, int *i, char *var, t_mini *mini)
{
	char	*s;
	int		j;

	j = 0;
	while (ft_isalnum(var[j]))
		j++;
	var[j] = '\0';
	s = expand_var(var, mini->export);
	free(var);
	if (s)
	{
		printf("%s", s);
		free(s);
	}
	while (ft_isalnum(input[*i]))
		(*i)++;
	return (s);
}
