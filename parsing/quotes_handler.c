#include "./../minishell.h"

int	check_unclosed_quotes(const char *line)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (line[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	if (in_single_quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed single quotes\n", 2);		return (1);
	}
	else if (in_double_quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed double quotes\n", 2);		return (1);
	}
	return (0);
}

char	*remove_quotes(char *str, t_token *token)
{
	int		len;
	int		end;
	char	*result;
	int		i;

	len = ft_strlen(str);
	end = len - 1;
	if (len < 2)
		return (str);
	if ((str[0] == '\'' && str[end] == '\'')
		|| (str[0] == '\"' && str[end] == '\"'))
	{
		token->quoted = 1;
		token->quoted_type = (str[0] == '\'') ? CMD_SINGLE_QUOTE : CMD_DOUBLE_QUOTE;
		i = 0;
		while (++i < end)
		{
			if (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == ';')
			{
				token->is_literal = 1;
				break ;
			}
		}
		result = malloc(len);
		if (!result)
			return (str);
		i = 0;
		while (++i < end)
			result[i - 1] = str[i];
		result[end - 1] = '\0';
		free(str);
		return (result);
	}
	return (str);
}

static void	check_literal_chars(t_token *token, int len)
{
	int	i;

	i = 1;
	while (i < len - 1)
	{
		if (token->cmd[i] == '|' || token->cmd[i] == '<' ||
			token->cmd[i] == '>' || token->cmd[i] == ';')
		{
			token->is_literal = 1;
			break ;
		}
		i++;
	}
}

void	process_quotes(t_token *token)
{
	int		len;
	char	*result;
	int		i;

	if (!token || !token->cmd)
		return ;
	len = ft_strlen(token->cmd);
	if (len >= 2 && ((token->cmd[0] == '"' && token->cmd[len - 1] == '"') ||
		(token->cmd[0] == '\'' && token->cmd[len - 1] == '\'')))
	{
		token->quoted = 1;
		token->quoted_type = (token->cmd[0] == '\'') ? CMD_SINGLE_QUOTE : CMD_DOUBLE_QUOTE;
		check_literal_chars(token, len);
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
}

int	handle_input_line(char *line, t_mini *ms)
{
	if (check_unclosed_quotes(line))
	{
		ms->exit_status = 2;
		return (0);
	}
	return (1);
}