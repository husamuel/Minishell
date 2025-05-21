/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:17:31 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/21 07:57:14 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	check_literal_chars(t_token *token, int len)
{
	int	i;

	i = 1;
	while (i < len - 1)
	{
		if (token->cmd[i] == '|' || token->cmd[i] == '<'
			|| token->cmd[i] == '>' || token->cmd[i] == ';')
		{
			token->is_literal = 1;
			break ;
		}
		i++;
	}
}

static void	ft_set_quote_type(t_token *token, int len)
{
	if (!token || !token->cmd || len < 2)
		return ;
	if ((token->cmd[0] == '"' && token->cmd[len - 1] == '"')
		|| (token->cmd[0] == '\'' && token->cmd[len - 1] == '\''))
	{
		token->quoted = 1;
		if (token->cmd[0] == '\'')
			token->quoted_type = CMD_SINGLE_QUOTE;
		else
			token->quoted_type = CMD_DOUBLE_QUOTE;
	}
}

static void	process_empty_quotes(t_token *token)
{
	char	*result;

	token->is_empty_quotes = 1;
	result = malloc(1);
	if (result)
	{
		result[0] = '\0';
		free(token->cmd);
		token->cmd = result;
	}
}

static void	process_quoted_content(t_token *token, int len)
{
	char	*result;
	int		i;

	result = malloc(len - 1);
	if (result)
	{
		i = 0;
		while (++i < len - 1)
			result[i - 1] = token->cmd[i];
		result[len - 2] = '\0';
		free(token->cmd);
		token->cmd = result;
	}
}

void	process_quotes(t_token *token)
{
	int		len;

	if (!token || !token->cmd)
		return ;
	len = ft_strlen(token->cmd);
	ft_set_quote_type(token, len);
	if (token->quoted)
	{
		check_literal_chars(token, len);
		if (len == 2 && ((token->cmd[0] == '"' && token->cmd[1] == '"')
				|| (token->cmd[0] == '\'' && token->cmd[1] == '\'')))
			process_empty_quotes(token);
		else
			process_quoted_content(token, len);
	}
}
