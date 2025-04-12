#include "minishell.h"

static char **free_mat(char **mat)
{
    int i;

    if (!mat)
        return NULL;
    i = 0;
    while (mat[i])
    {
        free(mat[i]);
        i++;
    }
    free(mat);
    return NULL;
}

t_token *create_new_token(char *cmd)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;

    new_token->cmd = (cmd) ? ft_strdup(cmd) : NULL;
    new_token->type = CMD_NONE;
    new_token->args_file = NULL;
    new_token->args = NULL;
    new_token->next = NULL;
    new_token->prev = NULL;
    new_token->quoted = 0;
    new_token->quoted_type = 0;
    new_token->is_invalid = 0;

    return new_token;
}



t_token *lexer(char *input)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    char **mat = ft_split(input, ' ');
    int i = 0;
    
    if (!mat)
        return NULL;
    while (mat[i])
    {
        t_token *new_token = create_new_token(mat[i]);
        if (!new_token)
        {
            while (head)
            {
                t_token *temp = head;
                head = head->next;
                free(temp->cmd);
                free(temp);
            }
            free_mat(mat);
            return NULL;
        }
        if (!head)
        {
            head = new_token;
            tail = new_token;
        }
        else
        {
            tail->next = new_token;
            new_token->prev = tail;
            tail = new_token;
        }
        i++;
    }
    free_mat(mat);
    return head;
}
