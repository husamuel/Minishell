#include "./../minishell.h"

char	**free_mat(char **mat)
{
	int	i;

	if (!mat)
		return (NULL);
	i = 0;
	while (mat[i])
	{
		free(mat[i]);
		i++;
	}
	free(mat);
	return (NULL);
}

t_token	*create_new_token(char *cmd)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->cmd = (cmd) ? ft_strdup(cmd) : NULL;
	new_token->type = CMD_NONE;
	new_token->args_file = NULL;
	new_token->args = NULL;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->quoted = 0;
	new_token->quoted_type = 0;
	new_token->is_invalid = 0;
	new_token->is_literal = 0;
	return (new_token);
}

static void	append_token(t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		new_token->prev = *tail;
		*tail = new_token;
	}
}

static void	free_token_list(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->cmd);
		free(temp);
	}
}

static t_token	*create_token_from_substr(char *input, int start, int end)
{
	char	*token_str;
	t_token	*new_token;

	token_str = ft_substr(input, start, end - start);
	new_token = create_new_token(token_str);
	free(token_str);
	return (new_token);
}

static t_token	*add_previous_token(t_token **head, t_token **tail,
									char *input, int *pos)
{
	t_token	*new_token;
	int		start;
	int		end;

	start = pos[0];
	end = pos[1];
	if (end <= start)
		return (NULL);
	new_token = create_token_from_substr(input, start, end);
	if (!new_token)
	{
		free_token_list(*head);
		return (NULL);
	}
	append_token(head, tail, new_token);
	return (new_token);
}

static int	handle_spaces(t_token **head, t_token **tail, char *input, 
						int *pos, int *in_quotes)
{
	int	i;
	int	*start;

	i = pos[1];
	start = &pos[0];
	if (!(*in_quotes) && input[i] == ' ')
	{
		if (i > *start)
		{
			pos[1] = i;
			if (!add_previous_token(head, tail, input, pos))
				return (0);
		}
		*start = i + 1;
	}
	return (1);
}

static int	handle_quote(int *in_quotes, char *quote_type, char current_char)
{
	if (current_char == '"' || current_char == '\'')
	{
		if (!(*in_quotes))
		{
			*in_quotes = 1;
			*quote_type = current_char;
		}
		else if (current_char == *quote_type)
		{
			*in_quotes = 0;
			*quote_type = '\0';
		}
	}
	return (1);
}

static int	create_operator_token(t_token **head, t_token **tail,
								char *input, int *i)
{
	char	op_str[3];
	t_token	*new_token;

	op_str[0] = input[*i];
	op_str[1] = 0;
	op_str[2] = 0;
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
	{
		op_str[1] = input[*i];
		(*i)++;
	}
	new_token = create_new_token(op_str);
	if (!new_token)
	{
		free_token_list(*head);
		return (0);
	}
	append_token(head, tail, new_token);
	return (1);
}

static int	handle_operators(t_token **head, t_token **tail, char *input,
						int *pos, int in_quotes)
{
	int	i;
	int	*start;

	i = pos[1];
	start = &pos[0];
	if (!in_quotes && (input[i] == '|' || input[i] == '<' || input[i] == '>'))
	{
		if (i > *start)
		{
			pos[1] = i;
			if (!add_previous_token(head, tail, input, pos))
				return (0);
		}
		if (!create_operator_token(head, tail, input, &i))
			return (0);
		*start = i + 1;
		pos[1] = i;
	}
	return (1);
}

static int	process_char(t_token **head, t_token **tail, char *input,
					int *pos, int *in_quotes, char *quote_type)
{
	int	i;

	i = pos[1];
	if (!handle_spaces(head, tail, input, pos, in_quotes))
		return (0);
	if (!handle_quote(in_quotes, quote_type, input[i]))
		return (0);
	if (!handle_operators(head, tail, input, pos, *in_quotes))
		return (0);
	return (1);
}

t_token	*lexer(char *input)
{
	t_token	*head;
	t_token	*tail;
	int		pos[2];
	int		in_quotes;
	char	quote_type;

	head = NULL;
	tail = NULL;
	in_quotes = 0;
	quote_type = '\0';
	pos[0] = 0;
	pos[1] = 0;
	while (input[pos[1]])
	{
		if (!process_char(&head, &tail, input, pos, &in_quotes, &quote_type))
			return (NULL);
		pos[1]++;
	}
	if (pos[1] > pos[0])
	{
		if (!add_previous_token(&head, &tail, input, pos))
			return (NULL);
	}
	return (head);
}