/* ************************************************************************** */
/* */
/* ::: :::::::: */
/* quotes_handler.c :+: :+: :+: */
/* +:+ +:+ +:+ */
/* By: gtretiak <marvin@42.fr> +#+ +:+ +#+ */
/* +#+#+#+#+#+ +#+ */
/* Created: 2025/04/27 15:34:54 by gtretiak #+# #+# */
/* Updated: 2025/04/27 16:12:39 by gtretiak ### ########.fr */
/* */
/* ************************************************************************** */
#include "./../minishell.h"

static void check_literal_chars(t_token *token, int len)
{
    int i;
    
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

static void ft_set_quote_type(t_token *token, int len)
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

void process_quotes(t_token *token)
{
    int len;
    char *result;
    int i;
    
    if (!token || !token->cmd)
        return ;
    len = ft_strlen(token->cmd);
    ft_set_quote_type(token, len);
    if (token->quoted)
    {
        check_literal_chars(token, len);
        if (len == 2 && 
            ((token->cmd[0] == '"' && token->cmd[1] == '"') || 
             (token->cmd[0] == '\'' && token->cmd[1] == '\'')))
        {
            token->is_empty_quotes = 1;
            result = malloc(1);
            if (result)
            {
                result[0] = '\0';
                free(token->cmd);
                token->cmd = result;
            }
        }
        else
        {
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
}