#include "./../minishell.h"

char *find_command_path(char *cmd, t_env *env)
{
    char *path_env;
    char **paths;
    char *full_path;
    int i;

    if (!cmd || access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));

    path_env = get_env_value(env, "PATH");
    if (!path_env)
        return (NULL);

    paths = ft_split(path_env, ':');
    if (!paths)
    {
        free(path_env);
        return (NULL);
    }

    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        if (!full_path)
        {
            ft_free_split(paths);
            free(path_env);
            return (NULL);
        }
        full_path = ft_strjoin_free(full_path, cmd);
        if (!full_path)
        {
            ft_free_split(paths);
            free(path_env);
            return (NULL);
        }
        if (access(full_path, X_OK) == 0)
        {
            ft_free_split(paths);
            free(path_env);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    ft_free_split(paths);
    free(path_env);
    return (NULL);
}

int token_has_redirection(t_token *token)
{
    t_token *curr = token->next;
    while (curr)
    {
        if (curr->type == CMD_REDIRECT_OUT || curr->type == CMD_REDIRECT_IN
            || curr->type == CMD_APPEND || curr->type == CMD_HEREDOC)
            return (1);
        curr = curr->next;
    }
    return (0);
}

int execute_token(t_token *token, t_mini *ms)
{
    if (!token || token->type == CMD_NONE)
        return (0);

    if (!is_valid_command(token))
    {
        ft_putstr_fd("minishell: command not found\n", 2);
        return (127);
    }

    if (token->type == CMD_BUILDIN || token->type == CMD_EXPR)
    {
        if (token_has_redirection(token))
            return (execute_external_builtin(token, ms));
        else
            return (execute_simple_command(token, ms));
    }
    else if (token->type == CMD_EXEC)
        return (execute_external_command(token, ms));

    return (0);
}
