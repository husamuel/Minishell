#include "minishell.h"

void add_to_args(t_token *token, char *arg)
{
    int i;
    char **new_args;

    i = 0;
    if (token->args)
    {
        while (token->args[i])
            i++;
    }
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return ;
    if (token->args)
    {
        i = 0;
        while (token->args[i])
        {
            new_args[i] = token->args[i];
            i++;
        }
        free(token->args);
    }
    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;
    token->args = new_args;
}

void add_to_args_file(t_token *token, char *arg)
{
    int i;
    char **new_args;

    i = 0;
    while (token->args_file[i])
        i++;
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return ;
    ft_memcpy(new_args, token->args_file, sizeof(char *) * i);
    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;
    free(token->args_file);
    token->args_file = new_args;
}

void handle_command_token(t_token *current, t_token **last_cmd,
    int *command_seen)
{
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
    if (is_builtin_command(current->cmd) || is_exec_command(current->cmd))
    {
        set_command_type(current);
        *command_seen = 1;
        *last_cmd = current;
    }
    else
    {
        current->type = CMD_NONE;
    }
}

void handle_exit_status_argument(t_token *current, t_token *last_cmd, t_mini *ms)
{
    char *exit_status_str;

    exit_status_str = ft_itoa(ms->exit_status);
    if (exit_status_str)
    {
        add_to_args_file(last_cmd, exit_status_str);
        add_to_args(last_cmd, exit_status_str);
        current->type = CMD_ARG;
        free(exit_status_str);
    }
}

void handle_argument_token(t_token *current, t_token *prev,
    t_token *last_cmd, t_mini *ms)
{
    char *processed_arg;

    (void)prev;
    
    if (!last_cmd)
        return ;
    
    if (ft_strcmp(current->cmd, "$?") == 0)
    {
        handle_exit_status_argument(current, last_cmd, ms);
        return;
    }
    
    processed_arg = ft_strdup(current->cmd);
    if (processed_arg)
    {
        add_to_args_file(last_cmd, processed_arg);
        add_to_args(last_cmd, processed_arg);
        current->type = CMD_ARG;
        free(processed_arg);
    }
}
