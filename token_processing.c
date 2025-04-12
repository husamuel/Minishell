#include "minishell.h"

static void setup_expr_command(t_token *current, int *command_seen,
t_token **last_cmd)
{
    current->type = CMD_EXPR;
    *command_seen = 1;
    *last_cmd = current;
    current->args_file = malloc(sizeof(char *) * 2);
    if (!current->args_file)
        return ;
    current->args_file[0] = ft_strdup(current->cmd);
    current->args_file[1] = NULL;
    current->args = malloc(sizeof(char *) * 2);
    if (!current->args)
        return ;
    current->args[0] = ft_strdup(current->cmd);
    current->args[1] = NULL;
}

static void setup_pipe_token(t_token *current, t_mini *ms)
{
    current->type = CMD_PIPE;
    ++ms->pipe;
}

static int is_redirect_out(char *cmd)
{
    return (cmd[0] == '>'
        || (ft_strlen(cmd) >= 2 && cmd[0] == '>' && cmd[1] == '>'));
}

static int is_redirect_in(char *cmd)
{
    return (cmd[0] == '<'
        || (ft_strlen(cmd) >= 2 && cmd[0] == '<' && cmd[1] == '<'));
}

static void setup_redirect_out_token(t_token *current, t_mini *ms)
{
    current->type = CMD_REDIRECT_OUT;
    ++ms->redirect;
}

static void setup_redirect_in_token(t_token *current, t_mini *ms)
{
    current->type = (current->cmd[1] == '<') ? CMD_HEREDOC : CMD_REDIRECT_IN;
    ++ms->redirect;
}

static int check_unclosed_quotes(const char *line)
{
    int in_single_quote;
    int in_double_quote;
    int i;

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
        ft_putstr_fd("minishell: erro de sintaxe: aspas simples não fechadas\n", 2);
        return (1);
    }
    else if (in_double_quote)
    {
        ft_putstr_fd("minishell: erro de sintaxe: aspas duplas não fechadas\n", 2);
        return (1);
    }
    return (0);
}

static char *remove_quotes(char *str, t_token *token)
{
    int len;
    int end;
    char *result;
    int i;

    len = ft_strlen(str);
    end = len - 1;
    if (len < 2)
        return (str);
    if ((str[0] == '\'' && str[end] == '\'')
        || (str[0] == '\"' && str[end] == '\"'))
    {
        token->quoted = 1;
        token->quoted_type = (str[0] == '\'') ? CMD_SINGLE_QUOTE : CMD_DOUBLE_QUOTE;
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

static void process_quotes(t_token *token)
{
    if (!token || !token->cmd)
        return ;
    token->cmd = remove_quotes(token->cmd, token);
}

static void process_exit_status(t_token *current, t_mini *ms)
{
    char *exit_status_str;

    current->type = CMD_EXIT_STATUS;
    free(current->cmd);
    exit_status_str = ft_itoa(ms->exit_status);
    current->cmd = exit_status_str;
    
    if (current->args)
    {
        free(current->args[0]);
        current->args[0] = ft_strdup(exit_status_str);
    }
    
    if (current->args_file)
    {
        free(current->args_file[0]);
        current->args_file[0] = ft_strdup(exit_status_str);
    }
}

static int is_in_expr_context(t_token *prev)
{
    if (prev && prev->cmd && ft_strcmp(prev->cmd, "expr") == 0)
        return (1);
    
    while (prev)
    {
        if (ft_strcmp(prev->cmd, "expr") == 0)
            return (1);
        if (prev->type == CMD_PIPE || prev->type == CMD_REDIRECT_IN || 
            prev->type == CMD_REDIRECT_OUT || prev->type == CMD_HEREDOC)
            break;
        prev = prev->prev;
    }
    
    return (0);
}


void process_token(t_token *current, t_token *prev, t_token **last_cmd,
    int *command_seen, t_mini *ms)
{
    process_quotes(current);
    if (prev && prev->cmd && prev->cmd[0] == '|')
    {
        *command_seen = 0;
        *last_cmd = NULL;
    }
    

    
    if (ft_strcmp(current->cmd, "$?") == 0 && !is_in_expr_context(prev))
    {
        process_exit_status(current, ms);
        if (!(*command_seen))
        {
            *command_seen = 1;
            *last_cmd = current;
            
            if (!current->args)
            {
                current->args = malloc(sizeof(char *) * 2);
                if (current->args)
                {
                    current->args[0] = ft_strdup(current->cmd);
                    current->args[1] = NULL;
                }
            }
            
            if (!current->args_file)
            {
                current->args_file = malloc(sizeof(char *) * 2);
                if (current->args_file)
                {
                    current->args_file[0] = ft_strdup(current->cmd);
                    current->args_file[1] = NULL;
                }
            }
        }
        else if (*last_cmd)
        {
            handle_argument_token(current, prev, *last_cmd, ms);
        }
        return;
    }
    else if (ft_strcmp(current->cmd, "$?") == 0 && is_in_expr_context(prev))
    {
        ms->exit_status_count++;
        if (*last_cmd)
        {
            handle_argument_token(current, prev, *last_cmd, ms);
        }
        return;
    }

    if (ft_strcmp(current->cmd, "expr") == 0)
    {
        setup_expr_command(current, command_seen, last_cmd);
        return;
    }

    if (current->cmd[0] == '|')
        setup_pipe_token(current, ms);
    else if (is_redirect_out(current->cmd))
        setup_redirect_out_token(current, ms);
    else if (is_redirect_in(current->cmd))
        setup_redirect_in_token(current, ms);
    else if (current->cmd[0] == '\\' || current->cmd[0] == ';')
        current->type = CMD_NONE;
    else if (!(*command_seen))
        handle_command_token(current, last_cmd, command_seen);
    else
        handle_argument_token(current, prev, *last_cmd, ms);
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